# Лабораторна робота №3

## Тема: Візуалізація та аналіз даних

Дана лабораторна робота присвячена аналізу та візуалізації даних із використанням бібліотек Python у середовищі Jupyter Notebook.

У роботі використано набір даних Adult Income Dataset з відкритого репозиторію UCI Machine Learning Repository. Датасет є багатовимірним (multivariate) та містить як числові, так і категоріальні атрибути, а також пропущені значення.


## Етапи роботи

1. Завантаження та первинний аналіз даних
2. Очищення даних (Data Cleaning):

   * обробка пропущених значень
   * видалення дублікатів
3. Візуалізація даних:

   * scatter plot
   * histogram (5 bins)
   * boxplot
   * heatmap
   * countplot
   * pairplot
   * багатовимірна візуалізація
4. Формування аналітичних висновків


## Інструкція із запуску

### Клонування репозиторію

```bash
git clone <URL>
```

### Перехід у папку

```bash
cd lab3
```

### Створення віртуального середовища

```bash
python -m venv venv
```

### Активація

```bash
venv\Scripts\activate
```

### Встановлення залежностей

```bash
pip install -r requirements.txt
```

### Запуск

```bash
jupyter notebook
```

Відкрити файл **DataVisual.ipynb** та виконати всі комірки.


## Використані технології

* Python
* Pandas
* NumPy
* Matplotlib
* Seaborn

### Версія Python

 **Python:** 3.12.6

### Посилання на датасет

Джерело: https://archive.ics.uci.edu/ml/datasets/adult
