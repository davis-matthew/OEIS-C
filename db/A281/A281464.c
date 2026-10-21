/*
 * A281464  Number of n X 3 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A281464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-3) - 3*a(n-4) - 4*a(n-5) + 4*a(n-6) -
 *            4*a(n-7) (for n > 9, zero‑based notation, table for 1 ≤ n ≤ 9
 *            below)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 54, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init9[9] = {
    4, 17, 24, 60, 133, 283, 634, 1419, 3092
};

static seq_t *cache      = NULL;   /* array of a(1)..a(cache_len) */
static size_t cache_cap  = 0;       /* allocated capacity */
static size_t cache_len  = 0;       /* number of terms stored */

/* Computes the n‑th term of A281464 (a(1) = 4). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safe guard for offset 1 */

    /* Return a precomputed value if available */
    if (n <= cache_len)
        return cache[n-1];

    /* Ensure initialized leading values up to at least 9 or n */
    if (cache_len < 9) {
        size_t required = n < 9 ? n : 9;
        if (cache_cap < required) {
            seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failed */
            cache = tmp;
            cache_cap = required;
        }
        for (size_t i = cache_len; i < required; ++i)
            cache[i] = init9[i];
        cache_len = required;
    }

    if (n <= 9)
        return cache[n-1];

    /* Allocate more space if needed */
    if (cache_cap < (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = n;
    }

    /* Apply the recurrence for all remaining terms */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        /* i corresponds to a(i+1) */
        seq_t val = 0;
        val += (seq_t)2 * cache[i-1];      /* a(i)   */
        val += (seq_t)3 * cache[i-3];      /* a(i-2) */
        val -= (seq_t)3 * cache[i-4];      /* a(i-3) */
        val -= (seq_t)4 * cache[i-5];      /* a(i-4) */
        val += (seq_t)4 * cache[i-6];      /* a(i-5) */
        val -= (seq_t)4 * cache[i-7];      /* a(i-6) */
        cache[i] = val;                    /* store a(i+1) */
    }
    cache_len = n;
    return cache[n-1];
}
