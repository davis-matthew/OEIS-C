/*
 * A391745  Odd numbers k such that 9*k = A048720(m, k) for some m, where
 *          A048720 is carryless base-2 multiplication.
 * https://oeis.org/A391745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   iterate odd k such that 9*k is divisible by k when the * product
 *            is interpreted as a carryless binary (GF(2)) multiplication
 * Range:     u8 n <= 7, u16 n <= 822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence values are computed lazily and memoized so that any
 * call order is efficient. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"

/* Helper: test if P is divisible by K in the polynomial ring GF(2)[x].
 * It performs a standard bitwise long division, XORing the divisor
 * shifted to align the leading 1’s of the dividend. */
static inline bool divisible_by_k(seq_t P, seq_t K)
{
    while (P != 0) {
        size_t p_len = bitlen_seq_t(P);
        size_t k_len = bitlen_seq_t(K);
        if (p_len < k_len)
            break;
        size_t shift = p_len - k_len;
        P ^= K << shift;
    }
    return P == 0;
}

/* Memoised sequence storage */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = (seq_t)1;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1 */

    /* Lazy initialisation of cache */
    if (cache == NULL) {
        cache_cap = 1024;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    }

    /* Extend cache until it contains the requested term */
    while (cache_len < (size_t)n) {
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        seq_t k = next_candidate;
        seq_t prod = ((seq_t)9) * k;    /* ordinary integer product */

        if (divisible_by_k(prod, k))
            cache[cache_len++] = k;

        /* next odd candidate */
        next_candidate += (seq_t)2;
    }

    return cache[n - 1];
}
