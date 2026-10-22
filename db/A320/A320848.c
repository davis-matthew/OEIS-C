/*
 * A320848  Number of 2-element subsets of [n] whose sum is a triangular
 *          number.
 * https://oeis.org/A320848
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2‑element subsets of [n] whose sum is a
 *            triangular number
 * Range:     u8 n <= 76, u16 n <= 3044, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t total = 0;
    seq_t maxT = (seq_t)2 * (seq_t)n - 1;
    seq_t T = 0;
    for (idx_t k = 1; ; ++k) {
        T += (seq_t)k;                     /* T_k = k*(k+1)/2, accumulated incrementally */
        if (T > maxT) break;
        seq_t L = (T > (seq_t)n) ? (T - (seq_t)n) : (seq_t)1; /* lower bound on i */
        seq_t R = (T - 1) / 2;                                  /* upper bound on i (i<j) */
        if (R >= L) {
            total += (R - L + 1);                               /* count pairs for this T_k */
        }
    }
    return total;
}
