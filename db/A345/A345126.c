/*
 * A345126  a(1) = 1; thereafter, a(n) = A073327(a(n-1)), using the British
 *          English version of A073327 when there is a choice.
 * https://oeis.org/A345126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = A073327(a(n-1)) for n > 1 */

#include <stddef.h>
#include <stdlib.h>

/* Sum of letter positions for the words “zero” … “nineteen”. */
static const idx_t onesSum[20] = {
    64, /* zero   */
    34, /* one    */
    58, /* two    */
    56, /* three  */
    60, /* four   */
    42, /* five   */
    52, /* six    */
    65, /* seven  */
    49, /* eight  */
    42, /* nine   */
    39, /* ten    */
    63, /* eleven */
    87, /* twelve */
    99, /* thirteen */
    104,/* fourteen */
    65, /* fifteen */
    96, /* sixteen */          /* fixed: was 85 */
    109,/* seventeen */
    73, /* eighteen */
    86  /* nineteen */
};

/* Sum of letter positions for the tens words “twenty” … “ninety”. */
static const idx_t tensSum[10] = {
    0,          /* placeholder for 0 */
    0,          /* placeholder for 1 */
    107,        /* twenty */
    100,        /* thirty */
    84,         /* forty  */
    66,         /* fifty  */
    97,         /* sixty  */
    110,        /* seventy*/
    74,         /* eighty */
    87          /* ninety */
};

#define HUNDRED_SUM 74   /* “hundred” */
#define AND_SUM     19   /* “and” */

/* Compute the sum of letter positions for the UK English name of x (x > 0). */
static seq_t letter_sum(idx_t x)
{
    if (x == 0)
        return 0;

    if (x < 20)
        return (seq_t)onesSum[x];

    if (x < 100) {
        if (x % 10 == 0)
            return (seq_t)tensSum[x / 10];
        /* add the units part */
        return (seq_t)(tensSum[x / 10] + onesSum[x % 10]);
    }

    /* 100 <= x < 1000 */
    int h = x / 100;          /* hundreds digit 1..9 */
    int r = x % 100;          /* remainder 0..99 */

    seq_t sum = (seq_t)(onesSum[h] + HUNDRED_SUM);

    if (r != 0) {
        sum += (seq_t)AND_SUM;
        sum += letter_sum((idx_t)r);
    }

    return sum;
}

/* Cached values of a(n). cache[i] holds a(i+1). */
static seq_t *cache   = NULL;
static idx_t  cache_n = 0;   /* number of elements stored */

/* Grow the cache if necessary and compute all missing terms up to n. */
static void ensure_terms(idx_t n)
{
    if (n <= cache_n)
        return;

    size_t new_cap = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) {
        abort();  /* Out of memory – terminate */
    }
    cache = tmp;

    for (idx_t i = cache_n; i < n; ++i) {
        if (i == 0)
            cache[0] = (seq_t)1;
        else
            cache[i] = letter_sum((idx_t)cache[i - 1]);
    }

    cache_n = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined by definition; return safe value */

    ensure_terms(n);
    return cache[n - 1];
}
