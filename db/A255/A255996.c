/*
 * A255996  Number of length n+5 0..1 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 15*n^2 + 290*n) / 6 + 12 for n > 3 a(1) = 64, a(2)
 *            = 128, a(3) = 188
 * Range:     u8 n <= 3, u16 n <= 67, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle small indices explicitly */
    if (n == 1) return (seq_t)64;
    if (n == 2) return (seq_t)128;
    if (n == 3) return (seq_t)188;
    /* compute the polynomial for n > 3 */
    uint64_t nn = (uint64_t)n;                     /* use 64‑bit temporary */
    uint64_t num = nn * nn * nn                    /* n^3   */
                 + 15 * nn * nn                    /* 15 n^2 */
                 + 290 * nn;                       /* 290 n  */
    seq_t result = (seq_t)(num / 6 + 12);
    return result;
}
