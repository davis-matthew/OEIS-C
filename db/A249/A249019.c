/*
 * A249019  Number of ternary words of length n in which all digits 0..2 occur
 *          in every 6 consecutive digits.
 * https://oeis.org/A249019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 3*a(n-3) + 5*a(n-4) + 6*a(n-5) -
 *            a(n-6) - a(n-7) - a(n-9) - a(n-10) for n ≥ 16 Initial values (n
 *            = 0 … 15): a(0)=1, a(1)=3, a(2)=9, a(3)=27, a(4)=81, a(5)=243,
 *            a(6)=540, a(7)=1440, a(8)=3804, a(9)=9960, a(10)=25908,
 *            a(11)=67344, a(12)=175884, a(13)=458832, a(14)=1196364...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init[16] = {
    (seq_t)1,  (seq_t)3,   (seq_t)9,   (seq_t)27,
    (seq_t)81, (seq_t)243, (seq_t)540, (seq_t)1440,
    (seq_t)3804, (seq_t)9960, (seq_t)25908, (seq_t)67344,
    (seq_t)175884, (seq_t)458832, (seq_t)1196364, (seq_t)3119304
};

static seq_t *cache = NULL;   /* memoised values */
static size_t cache_cap = 0;  /* current capacity of cache */

/* Ensure the cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;                   /* already enough */

    size_t old_cap = cache_cap;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)                      /* allocation failure – keep return 0 */
        return;
    cache = tmp;
    cache_cap = new_cap;

    for (size_t i = old_cap; i < new_cap; ++i) {
        if (i < 16) {
            cache[i] = _init[i];
        } else {
            /* recurrence for i ≥ 16 */
            seq_t val = cache[i-1];
            val += cache[i-2] * 2;
            val += cache[i-3] * 3;
            val += cache[i-4] * 5;
            val += cache[i-5] * 6;
            val -= cache[i-6];
            val -= cache[i-7];
            val -= cache[i-9];
            val -= cache[i-10];
            cache[i] = val;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 16)
        return _init[(size_t)n];

    ensure_cache(n);
    return cache[(size_t)n];
}
