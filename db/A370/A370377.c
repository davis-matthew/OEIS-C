/*
 * A370377  a(n) is the number of symmetrical linear hydrocarbon chains with n
 *          C-C bonds.
 * https://oeis.org/A370377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-2) + a(n-4) - a(n-6) for n ≥ 6 Base values: a(0)=1,
 *            a(1)=3, a(2)=2, a(3)=6, a(4)=5, a(5)=14 Recurrence derived from
 *            the generating function (1+3*x-x^5)/(1-2*x^2-x^4+x^6)
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 54, u64 n <= 108,
 *            u128 n <= 218, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memory for computed values */
static size_t capacity = 0;          /* allocated size of cache */
static idx_t next_computed = 6;      /* next index that needs to be filled */

/* Ensure that cache[n] is valid and computed */
static void ensure_cache(idx_t n) {
    if (cache == NULL) {
        /* initial allocation large enough for first call */
        capacity = n + 1 > 32 ? (size_t)(n + 1) * 2 : 32;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        /* initialize base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)14;
        next_computed = 6;
    } else if ((size_t)(n + 1) > capacity) {
        size_t new_cap = (size_t)(n + 1) > capacity * 2 ? (size_t)(n + 1) : capacity * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }
    /* compute new terms using the recurrence */
    if (next_computed <= n) {
        for (idx_t i = next_computed; i <= n; ++i) {
            cache[i] = ((seq_t)2 * cache[i - 2]) + cache[i - 4] - cache[i - 6];
        }
        next_computed = n + 1;
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
