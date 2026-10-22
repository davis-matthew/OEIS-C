/*
 * A382126  G.f. satisfies A(x) = A(x^2)*A(x^3) / (1-x).
 * https://oeis.org/A382126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + sum_{i>=0, j>=0, 2*i+3*j=n} a(i)*a(j); a(0)=1
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 157, u64 n <= 566,
 *            u128 n <= 1030, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for the sequence values. */
static seq_t *A_cache = NULL;
static idx_t A_size  = 0;   /* Number of entries currently computed (next unwritten index). */
static idx_t A_cap   = 0;   /* Current allocation capacity. */

/* Ensure that the cache has capacity at least n+1. */
static void ensure_capacity(idx_t n)
{
    if (n < A_cap) return;
    idx_t newcap = A_cap ? A_cap << 1 : 4;
    while (newcap <= n) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(A_cache, newcap * sizeof(seq_t));
    if (!tmp) abort();                       /* Allocation failed. */
    A_cache = tmp;
    A_cap   = newcap;
}

/* Fill the cache up to index n (inclusive). */
static void compute_up_to(idx_t n)
{
    if (A_size > n) return;                 /* Already computed. */
    for (idx_t cur = A_size; cur <= n; ++cur) {
        if (cur == 0) {
            A_cache[0] = (seq_t)1;
        } else {
            seq_t sum = 0;
            for (idx_t i = 0; i <= cur >> 1; ++i) {     /* i <= cur/2 */
                idx_t rem = cur - (i << 1);             /* cur - 2*i */
                if (rem % 3 == 0) {
                    idx_t j = rem / 3;
                    sum += A_cache[i] * A_cache[j];
                }
            }
            A_cache[cur] = A_cache[cur - 1] + sum;
        }
    }
    A_size = n + 1;
}

/* Public function implementing the sequence. */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    compute_up_to(n);
    return A_cache[n];
}
