/*
 * A247617  a(4n) = n + 1/2 - (-1)^n/2 + (-1)^n, a(2n+1) = 2*n + 5, a(4n+2) =
 *          2*n + 3.
 * https://oeis.org/A247617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = n + 4 if n is odd, * a(n) = n/2 + 2 if n ≡ 2 (mod 4), *
 *            a(n) = n/4 + 1 (n/4 even) if n ≡ 0 (mod 8), * a(n) = n/4 if n ≡
 *            4 (mod 8). * These cases are equivalent to the OEIS definitions:
 *            * a(4n) = n + 1/2 - (-1)^n/2 + (-1)^n * a(2n+1) = 2n + 5 *
 *            a(4n+2) = 2n + 3
 * Range:     u8 n <= 252, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Odd indices: a(2k+1) = 2k + 5 = n + 4 */
    if (n & 1U) {
        return (seq_t)n + 4;
    }

    /* Even indices */
    if ((n & 3U) == 0U) {        /* n ≡ 0 (mod 4) */
        idx_t k = n >> 2;       /* k = n/4 */
        if ((k & 1U) == 0U) {   /* k even → n ≡ 0 (mod 8) */
            return (seq_t)k + 1;
        } else {                /* k odd  → n ≡ 4 (mod 8) */
            return (seq_t)k;
        }
    }

    /* n ≡ 2 (mod 4) */
    return (seq_t)(n >> 1) + 2;  /* n/2 + 2 */
}
