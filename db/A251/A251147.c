/*
 * A251147  Number of (n+1) X (6+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 4 and no 2 X 2 subblock having exactly two nonzero
 *          entries.
 * https://oeis.org/A251147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 12, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A251147
    a(n) = 3*a(n-1) - 5*a(n-3) + a(n-4) + 2*a(n-5)
    a(1) = 427, a(2) = 565, a(3) = 777, a(4) = 1141, a(5) = 1743
    (sequence defined for n ≥ 1) */

#include <stdlib.h>   /* malloc, free, realloc */

seq_t a(idx_t n)
{
    /* Static cache holds values starting at a(1) in cache[0] */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0; /* capacity in elements */
    static size_t cache_size = 0;/* number of elements actually stored */

    if (n == 0)                 /* sequence undefined for n = 0 */
        return 0;

    /* If requested value already cached, return it */
    if (cache_size >= (size_t)n)
        return cache[n - 1];

    /* Grow cache to at least n elements */
    size_t need = (size_t)n;
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < need)
            new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing values sequentially using the recurrence */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 1)       val = 427;
        else if (i == 2)  val = 565;
        else if (i == 3)  val = 777;
        else if (i == 4)  val = 1141;
        else if (i == 5)  val = 1743;
        else {
            /* a(i) = 3*a(i-1) - 5*a(i-3) + a(i-4) + 2*a(i-5) */
            seq_t a_nm1 = cache[i - 2]; /*  a(i-1) */
            seq_t a_nm3 = cache[i - 4]; /*  a(i-3) */
            seq_t a_nm4 = cache[i - 5]; /*  a(i-4) */
            seq_t a_nm5 = cache[i - 6]; /*  a(i-5) */

            seq_t part1 = (seq_t)3 * a_nm1;
            seq_t part2 = (seq_t)5 * a_nm3;
            seq_t part3 = a_nm4;
            seq_t part4 = (seq_t)2 * a_nm5;

            val = part1 + part3 + part4 - part2;
        }
        cache[i - 1] = val;
    }

    cache_size = (size_t)n;
    return cache[n - 1];
}
