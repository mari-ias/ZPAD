# IMPORTS
import streamlit as st
import pandas as pd
import plotly.express as px
import os

# 1. CONFIGURATION
st.set_page_config(
    page_title="VHI Data Explorer | FB-42 Syzova Mariia",
    page_icon="🌾",
    layout="wide"
)

# 2. CONSTANTS 
NOAA_ID_TO_NAME = {
    1: "Вінницька", 2: "Волинська", 3: "Дніпропетровська", 4: "Донецька",
    5: "Житомирська", 6: "Закарпатська", 7: "Запорізька", 8: "Івано-Франківська",
    9: "Київська", 10: "Кіровоградська", 11: "Луганська", 12: "Львівська",
    13: "Миколаївська", 14: "Одеська", 15: "Полтавська", 16: "Рівненська",
    17: "Сумська", 18: "Тернопільська", 19: "Харківська", 20: "Херсонська",
    21: "Хмельницька", 22: "Черкаська", 23: "Чернівецька", 24: "Чернігівська",
    25: "Республіка Крим"
}

DATA_FOLDER = "data"


# 3. DATA LOADING 
@st.cache_data
def load_and_clean_data(folder_path):
    all_data = []

    if not os.path.exists(folder_path):
        return pd.DataFrame()

    files = sorted(f for f in os.listdir(folder_path) if f.endswith(".csv"))

    for file in files:
        try:
            noaa_id = int(file.split("_")[2])
            filepath = os.path.join(folder_path, file)

            headers = ['year', 'week', 'SMN', 'SMT', 'VCI', 'TCI', 'VHI', 'empty']

            df = pd.read_csv(filepath, skiprows=2, names=headers)

            # cleaning
            df = df.drop(columns=['empty'], errors='ignore').dropna()

            df['year'] = (
                df['year']
                .astype(str)
                .str.replace('<pre>', '', regex=False)
                .str.replace('</pre>', '', regex=False)
            )

            df = df[df['year'].str.isnumeric()]

            df = df.astype({
                'year': int,
                'week': int,
                'VCI': float,
                'TCI': float,
                'VHI': float
            })

            df['region'] = NOAA_ID_TO_NAME.get(noaa_id, f"Region {noaa_id}")
            df = df[df["region"].isin(NOAA_ID_TO_NAME.values())]

            all_data.append(df)

        except Exception:
            continue

    if not all_data:
        return pd.DataFrame()

    return pd.concat(all_data, ignore_index=True)


df = load_and_clean_data(DATA_FOLDER)


# 4. SIDEBAR 
st.sidebar.header("🔧 Фільтри")

if df.empty:
    st.error("Дані не знайдено. Перевір папку data/")
    st.stop()

index_type = st.sidebar.selectbox(
    "Індекс",
    ["VHI", "VCI", "TCI"]
)

region_choice = st.sidebar.selectbox(
    "Область",
    sorted(df["region"].unique())
)

week_range = st.sidebar.slider(
    "Тижні",
    1, 52,
    (1, 52)
)

year_range = st.sidebar.slider(
    "Роки",
    int(df["year"].min()),
    int(df["year"].max()),
    (int(df["year"].min()), int(df["year"].max()))
)

st.sidebar.subheader("Сортування")
sort_asc = st.sidebar.checkbox("↑ За зростанням")
sort_desc = st.sidebar.checkbox("↓ За спаданням")

if st.sidebar.button("🔄 Reset"):
    st.rerun()


# 5. FILTERING 
filtered = df[
    (df["region"] == region_choice) &
    (df["year"].between(*year_range)) &
    (df["week"].between(*week_range))
]

if filtered.empty:
    st.warning("Немає даних для обраних параметрів")
    st.stop()

# sorting logic
if sort_asc and sort_desc:
    st.sidebar.warning("Обрано обидва варіанти — сортування не застосовано")
elif sort_asc:
    filtered = filtered.sort_values(by=index_type, ascending=True)
elif sort_desc:
    filtered = filtered.sort_values(by=index_type, ascending=False)


# 6. MAIN UI 
st.title("🌾 Аналіз агрокліматичних показників України")
st.markdown(f"### Область: **{region_choice}** | Індекс: **{index_type}**")


# 7. TABS 
tab1, tab2, tab3 = st.tabs(["📊 Таблиця", "📈 Графік", "🌍 Порівняння"])

# TABLE 
with tab1:
    st.subheader("Дані")
    st.dataframe(filtered, use_container_width=True)

# MAIN CHART 
with tab2:
    st.subheader("Динаміка по роках")

    fig = px.line(
        filtered,
        x="week",
        y=index_type,
        color="year",
        markers=True,
        line_shape="spline",
        labels={
            "week": "Тиждень",
            index_type: index_type
        }
    )

    fig.update_layout(hovermode="x unified")

    st.plotly_chart(fig, use_container_width=True)

# COMPARISON 
with tab3:
    st.subheader("Порівняння регіонів")

    compare_df = df[
        (df["year"].between(*year_range)) &
        (df["week"].between(*week_range))
    ]

    fig2 = px.line(
        compare_df,
        x="week",
        y=index_type,
        color="region",
        labels={
            "week": "Тиждень",
            "region": "Область"
        }
    )

    fig2.update_layout(hovermode="x unified")

    st.plotly_chart(fig2, use_container_width=True)


# FOOTER 
st.markdown("---")
st.caption("Лабораторна робота №5 | ФБ-42 | Сизова Марія")
