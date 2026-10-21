/*
 * A261480  Number of set partitions of [n] into exactly four parts such that
 *          no part contains two elements with a circular distance less than
 *          three.
 * https://oeis.org/A261480
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a_{n-2} + 5*a_{n-3} + 4*a_{n-4} - a_{n-5} - 4*a_{n-6} -
 *            4*a_{n-7} (n ≥ 8) with initial values a(4)=1, a(5)=0, a(6)=3,
 *            a(7)=7. Offset: 4
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* array of computed values   */
static size_t cache_len = 0;         /* size of allocated array    */

static void init_cache(void)
{
    if (cache_len != 0) return;
    const size_t init_size = 8;      /* indices 0 … 7   */
    cache = (seq_t *)malloc(init_size * sizeof(seq_t));
    if (!cache) return;
    for (size_t i = 0; i < init_size; ++i) cache[i] = 0;
    cache[4] = (seq_t)1;
    cache[5] = (seq_t)0;
    cache[6] = (seq_t)3;
    cache[7] = (seq_t)7;
    cache_len = init_size;
}

static void ensure_size(idx_t n)
{
    if (n < 4) return;                /* below the offset nothing is needed */
    size_t required = (size_t)n + 1;
    if (cache_len >= required) return;

    size_t new_len = required;
    if (new_len < 8) new_len = 8;      /* reserve indices up to 7   */

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return;                 /* allocation failed – keep old data  */
    cache = tmp;
    for (size_t i = cache_len; i < new_len; ++i) cache[i] = 0;

    /* set base values if they have not yet been set */
    if (cache_len <= 4 && new_len > 4) cache[4] = (seq_t)1;
    if (cache_len <= 5 && new_len > 5) cache[5] = (seq_t)0;
    if (cache_len <= 6 && new_len > 6) cache[6] = (seq_t)3;
    if (cache_len <= 7 && new_len > 7) cache[7] = (seq_t)7;

    /* fill in values using the recurrence */
    size_t start = (cache_len > 8 ? cache_len : 8);
    for (size_t i = start; i < new_len; ++i) {
        seq_t t = cache[i-2];
        t += (seq_t)5 * cache[i-3];
        t += (seq_t)4 * cache[i-4];
        t -= cache[i-5];
        t -= (seq_t)4 * cache[i-6];
        t -= (seq_t)4 * cache[i-7];
        cache[i] = t;
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;   /* indices below the offset are undefined */
    ensure_size(n);
    if (!cache) return (seq_t)0;   /* allocation failure – return 0 */
    return cache[(size_t)n];
}
