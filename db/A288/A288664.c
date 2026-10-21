/*
 * A288664  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 493", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n >= 0 a(0) = 1 a(1) = 1 if n is even (n = 2k, k >= 1) a(n)
 *            = 2^(k+1) * (2^k - 1) if n is odd (n = 2k+1, k >= 1) a(n) =
 *            2^(k+1) - 1 (These formulas were derived from the recurrence
 *            a(n) = 7*a(n-2) - 14*a(n-4) + 8*a(n-6) for n > 6.)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2u.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    if ((n & 1) == 0) {          /* n even: n = 2k, k >= 1 */
        idx_t k = n >> 1;
        seq_t p1 = pow2u(k + 1);            /* 2^(k+1) */
        seq_t p2 = pow2u(k);                /* 2^k   */
        return p1 * (p2 - (seq_t)1);        /* 2^(k+1)*(2^k - 1) */
    } else {                        /* n odd:  n = 2k+1, k >= 1 */
        idx_t k = (n + 1) >> 1;
        seq_t p = pow2u(k);                /* 2^k   */
        return p - (seq_t)1;               /* 2^k - 1  */
    }
}
