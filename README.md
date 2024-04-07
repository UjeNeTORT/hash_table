# hash_table
---
Хэш-таблица - одна из основных структур данных, используемых в разработке. Ее преимущества - быстрый поиск и вставка.

В этой работе я:
- [x] Написал свой вариант хэш-таблицы
- [x] Проанализировал несколько хэш-функций и выбрал оптимальную
- [ ] Выполнил 3 ассемблерные оптимизации, ускорившие работу в ??? раз

## Общие сведения по хэш-таблицам
Хэш таблицы хранят пары ключ-значение, доступ к значению производится по ключу путем вычисления хэш функции $h(key)$, - в идеальном мире такое значение должно быть уникальным для каждого уникального $key$ и восроизводимым для одинаковых $key$.
В реальности же, часто бывает так, что возникает **коллизия**: $h(key1) = h(key2), key1 \neq key2$

### Разрешение коллизий

Существует несколько способов разрешения коллизий:

- хэш-таблица с открытой адресацией
- хэш таблица на списках

В моей работе используется вариант с двусвязными списками:

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="img/hash_table_dark.png">
  <source media="(prefers-color-scheme: light)" srcset="img/hash_table_light.png">
  <img alt="shows diagram with hash table working principles." src="img/hash_table_light.png">
</picture>

## Измерение производительности


## Источники
text taken from https://github.com/kaiwang0112006/myebooks/blob/master/content/George_RR_Martin-Ice_and_Fire_books_1-4/Martin_George_RR-Ice_and_Fire_4-A_Feast_for_Crows/Martin,%20George%20RR%20-%20Ice%20and%20Fire%204%20-%20A%20Feast%20for%20Crows.txt