/*
 * A368489  a(n) = Sum_{k=0..n} n^k * binomial(k+n,k).
 * https://oeis.org/A368489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} n^k * binomial(k+n, k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the recurrence
   term_0 = 1
   term_k = term_{k-1} * n * (n + k) / k
   and accumulates the sum. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum  = 1;       /* term for k = 0 */
    seq_t term = 1;

    for (idx_t k = 1; k <= n; ++k) {
#ifdef __cplusplus
        /* Big‑integer arithmetic (arbitrary precision builds). */
        term = (term * (seq_t)n * (seq_t)(n + k)) / (seq_t)k;
#else
        /* 128‑bit intermediate to prevent overflow in 64‑bit builds. */
        unsigned __int128 prod = (unsigned __int128)term *
                                 (unsigned __int128)n *
                                 (unsigned __int128)(n + k);
        term = (seq_t)(prod / k);
#endif
        sum += term;
    }
    return sum;
}
