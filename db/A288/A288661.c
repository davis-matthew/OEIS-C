/*
 * A288661  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 493", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 for n > 0: if n is even let k = n/2, a(n) = (10^k -
 *            1)/9 // repunit of length k if n is odd let k = (n+1)/2, a(n) =
 *            ((10^k - 1)/9) * 10^k // repunit of length k followed by k zeros
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"   /* provides seq_t pow10(idx_t e) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if ((n & 1ULL) == 0) {                  /* even n */
        idx_t k = n >> 1;                   /* k = n/2 */
        seq_t p10 = pow10(k);               /* 10^k */
        return (p10 - (seq_t)1) / (seq_t)9; /* (10^k - 1) / 9 */
    }
    else {                                  /* odd n */
        idx_t k = (n + 1ULL) >> 1;          /* k = (n+1)/2 */
        seq_t p10 = pow10(k);               /* 10^k */
        seq_t rep = (p10 - (seq_t)1) / (seq_t)9; /* (10^k - 1) / 9 */
        return rep * p10;                   /* repunit followed by k zeros */
    }
}
