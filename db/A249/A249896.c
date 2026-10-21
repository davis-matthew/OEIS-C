/*
 * A249896  a(n+1) is next smallest square not divisible by 10 beginning with
 *          a(n), initial term is 6.
 * https://oeis.org/A249896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=6; a(n)=next smallest square not divisible by 10 that
   starts with a(n-1). */
#include <stdlib.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt */
#include "../utils/pow10.h"       /* pow10 */

static seq_t *cache = NULL;      /* 1‑based indexing */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms stored (0 if empty) */

static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= (size_t)n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

static seq_t next_square(seq_t prev)
{
    seq_t tenpow = pow10((idx_t)1);          /* 10^1 */
    while (1) {
        seq_t low  = prev * tenpow;
        seq_t high = (prev + (seq_t)1) * tenpow - (seq_t)1; /* inclusive */

        seq_t m = floor_sqrt(low);
        if (m * m < low) m = m + (seq_t)1;      /* first m with m^2 >= low */

        seq_t sqrt_high = floor_sqrt(high);

        /* Scan from m upward until a suitable square is found. */
        for (; m <= sqrt_high; ++m) {
            if ((m % (seq_t)10) == (seq_t)0) continue;    /* skip multiples of 10 */
            seq_t sq = m * m;
            if (sq >= low && sq <= high) return sq;
        }
        /* No candidate found for this number of trailing zeros; increase it. */
        tenpow = tenpow * (seq_t)10;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;          /* undefined for n<1 */
    ensure_cache(n);
    if (cache_len > (size_t)n) return cache[(size_t)n];

    size_t start = cache_len;
    if (start == 0) start = 1;                  /* start from the first missing term */

    for (size_t i = start; i <= (size_t)n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)6;
        } else {
            cache[i] = next_square(cache[i-1]);
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
