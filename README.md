# hash_table
---
Хэш-таблица - одна из основных структур данных, используемых в разработке. Ее преимущества - быстрый поиск и вставка.

В этой работе я:
- [x] Написал свой вариант хэш-таблицы
- [x] Проанализировал несколько хэш-функций и выбрал оптимальную
- [ ] Выполнил 3 ассемблерные оптимизации, ускорившие работу в ??? раз

## Общие сведения по хэш-таблицам
Хэш таблицы хранят пары ключ-значение, доступ к значению производится по ключу путем вычисления хэш функции $h(key)$. Таким образом мы имеем доступ к элементу за О(1). В идеальном мире значение $h(key)$ должно быть уникальным для каждого уникального $key$ и восроизводимым для одинаковых $key$.
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

В случае коллизии помещаем значение в список, соответствующий $h(key)$ ячейке хэш-таблицы.
Поиск по списку существенно медленнее, чем по хэш-таблице ($O(n) ~против ~O(1)$).

Видно, что если хэш-функция часто будет возвращать одинаковые значения, часть хэш-таблицы будет пустой, в то время как другая часть будет перегружена длинными списками, что сведет на нет всю выгоду от использования хэш-таблицы.

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="img/good_bad_ht_dark.png">
  <source media="(prefers-color-scheme: light)" srcset="img/good_bad_ht_light.png">
  <img alt="shows diagram with hash table working principles." src="img/good_bad_ht_light.png">
</picture>

**Ключевое влияние на эффективность хэш-таблицы оказывает выбранная хэш-функция.**

## Сравнение хэш-функций
Во всех случаях использовался препроцессированный текст за авторством *Martin George RR "Ice and Fire 4 - A Feast for Crows"*, взятый с [этого гитхаба](https://github.com/kaiwang0112006/myebooks/blob/master/content/George_RR_Martin-Ice_and_Fire_books_1-4/).

В препроцессированном файле каждое слово расположено на своей строке в нижнем регистре без знаков препинания для удобства чтения.

Анализ включает в себя построение гистограмм заселенности и тест производительности.

[Перейти к результатам](#return-0)

### Методика измерений
Для построения гистограмм заселенности в хэш-таблицу загружались слова из целевого текста ('target_data'). Для каждого списка сохранялась его длина.

Для анализа производительности в хэш-таблицу загружались слова из целевого текста. Дальше в буфер загружались слова из другого набора ('test_cases'). В файл с результатами сохранялись значения среднего времени поиска $\mu$, стандартного отклонения $\sigma^2$, минимальное время поиска и максимальное время поиска для каждого теста.

По результатам тестирования строилась гистограмма

### Return 0
$h(key) = 0$ при любых входных данных.

Данная хэш функция очевидно плоха, т.к. помещает все значения в один и тот же список. В этом случае мы не видим пользы от хэш-таблицы т.к. эта ситуация эквивалентна созданию одного единственного списка.

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="img/load_hists/retzero_hf_dark_background.png">
  <source media="(prefers-color-scheme: light)" srcset="img/load_hists/retzero_hf_ggplot.png">
  <img alt="shows diagram with hash table working principles." src="img/load_hists/retzero_hf_ggplot.png">
</picture>

### ASCII первой буквы
$h(key) = ASCII(key[0])$ - ascii код первой буквы слова key

Эта хэш функция лучше предыдущей, т.к. существуют данные для которых она вернет разные значения, но она все равно плохая, потому что:
- распределение букв с которых могут начинаться слова неравномерное (на букву s начинается больше слов чем на z)
- мы можем получить только 26 уникальных значений - число букв в алфавите


<picture>
  <source media="(prefers-color-scheme: dark)" srcset="img/load_hists/ascii_first_hf_dark_background.png">
  <source media="(prefers-color-scheme: light)" srcset="img/load_hists/ascii_first_hf_ggplot.png">
  <img alt="shows diagram with hash table working principles." src="img/load_hists/ascii_first_hf_ggplot.png">
</picture>

### Length (key)
$h(key) = length(key)$ - длина слова key

Плохая хэш функция, т.к. длина слова в английском языке редко превышает 10 *(в моем наборе данных максимальная длина слова была 36)* - в этом смысле такая хэш-функция хуже предыдущей.


<picture>
  <source media="(prefers-color-scheme: dark)" srcset="img/load_hists/wd_len_hf_dark_background.png">
  <source media="(prefers-color-scheme: light)" srcset="img/load_hists/wd_len_hf_ggplot.png">
  <img alt="shows diagram with hash table working principles." src="img/load_hists/wd_len_ggplot.png">
</picture>

### CheckSum (контрольная сумма)
$h(key) = sum(key)$ - сумма ASCII кодов букв слова key

Популярная хэш функция из-за своей простоты и относительно хороших показателей дисперсии, но у этой функции есть подводный камень.


<picture>
  <source media="(prefers-color-scheme: dark)" srcset="img/load_hists/ctrl_sum_hf_dark_background.png">
  <source media="(prefers-color-scheme: light)" srcset="img/load_hists/ctrl_sum_hf_ggplot.png">
  <img alt="shows diagram with hash table working principles." src="img/load_hists/ctrl_sum_ggplot.png">
</picture>

Подводный камень:
**давайте увеличим размер хэш табицы:**


<picture>
  <source media="(prefers-color-scheme: dark)" srcset="img/load_hists/ctrl_sum_hf_long_dark_background.png">
  <source media="(prefers-color-scheme: light)" srcset="img/load_hists/ctrl_sum_hf_long_ggplot.png">
  <img alt="shows diagram with hash table working principles." src="img/load_hists/ctrl_sum_hf_long_ggplot.png">
</picture>

Расределение стало очень неравномерным, связано это с тем что сумма ascii кодов любого слова почти никогда не превышает какое-то значение из-за ограничений нашего языка.

### ROR hash function
$h(key) = ???$

### CRC32 - cycle redundancy check
$h(key) = crc32(key)$ - эта функция используется для коррекции ошибок, вызванных шумом, при передаче данных. Внутри себя она считает проверочное значение (check value). Функция, которая его считает часто используется как хэш-функция.

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="img/load_hists/wd_len_hf_dark_background.png">
  <source media="(prefers-color-scheme: light)" srcset="img/load_hists/wd_len_hf_ggplot.png">
  <img alt="shows diagram with hash table working principles." src="img/load_hists/wd_len_ggplot.png">
</picture>

Распределение выглядит хорошо: оно равномерно и нет больших незаселенных участков.

*В дальнейшем я буду использовать именно эту хэш-функцию.*

## Измерение производительности

Задача была оптимизировать хэш-таблицу под поиск, поэтому тестировать будем функцию поиска.

Хэш-таблица изначально хранит данные по словам из книги Джорджа Мартина (см. выше), искать слова мы будем не из этого же набора, чтобы встречались ситуации "ненахода" слова в хэш-таблице.

**Данные для заполнения хэш-таблицы:**  слова из книги Джорджа Мартина ([источник](https://github.com/kaiwang0112006/myebooks/blob/master/content/George_RR_Martin-Ice_and_Fire_books_1-4/))

**Данные для тестирования:** список английских слов ([источник](https://github.com/dwyl/english-words/blob/master/words.txt))

Время работы функции замерялось с помощью ассемблерной вставки инструкции 'rdtsc'.

### Профилирование

TODO

## Оптимизации

Хоть и изначальной целью работы было ускорение хэш-таблицы с помощью ассемблерных оптимизаций, в реальной практике к ним стоит относиться осторожно и использовать только если они дают значимую выгоду, так как они ухудшают переносимость и читаемость кода.

Поэтому перед выполнением ассемблерных оптимизаций, я выполнил внеассемблерные.

### Внеассемблерные оптимизации

Самые очевидные внеассемлерные оптимизации - выключить assertы и верификаторы структур данных, скомпилировать с ключами '-O2', '-O3'

Результаты приведены в таблице:

| Оптимизация                                 | Относительное ускорение среднего времени поиска, % |
| :------------------------------------------ | :------------------------------------------------: |
| '-O1'                                       | 1                                                  |
| '-O1' + отключение 'assert'                 | 104.8                                              |
| '-O1' + отключение 'assert' и верификаторов | 148.8                                              |
| '-O2' + отключение 'assert' и верификаторов | 160.3                                              |
| '-O3' + отключение 'assert' и верификаторов | 137.9                                              |

## Источники
