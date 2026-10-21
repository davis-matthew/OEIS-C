/*
 * A290954  The number of permutations in S_n for which the number of reduced
 *          words is minimized with respect to the numbers of braid and
 *          commutation classes: |R(w)| = |B(w)| + |C(w)| - 1.
 * https://oeis.org/A290954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n) + (n^3 - 3n^2 + 8n - 21) / 3 for n>2, with a(1) = 1
 *            and a(2) = 2. C(n) is the nth Catalan number
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 2;

    seq_t ns = (seq_t)n;
    seq_t ns2 = ns * ns;           /* n^2 */
    seq_t ns3 = ns2 * ns;          /* n^3 */

    /* Compute the polynomial n^3 - 3n^2 + 8n - 21 */
    seq_t poly = ns3 - 3 * ns2 + 8 * ns - 21;

    /* Divide by 3 (divisible for n > 2) */
    seq_t extra = poly / 3;

    /* Add the Catalan number */
    seq_t result = catalan(n) + extra;
    return result;
}
