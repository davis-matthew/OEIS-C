/*
 * A286866  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 814", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A286866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 1 * a(1) = 2 * For n ≥ 2 let k = ⌊log₂ n⌋, s = 2^(k−1).
 *            * Then a(n) = (2^s − 1) · 2^( n − (s − 1) )
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;

    /* floor(log2 n) – k is the index of the highest set bit (n>0) */
    int k = 63 - __builtin_clzll(n);        /* works for idx_t = uint64_t */

    /* s = 2^(k-1) ;  k ≥ 1 because n ≥ 2 */
    idx_t s = 1ULL << (k - 1);

    /* (2^s – 1)  */
    seq_t mult = pow2(s) - 1;

    /* remaining factor: 2^( n - (s-1) )  */
    idx_t rem_exp = n - (s - 1);
    seq_t pow_rem = pow2(rem_exp);

    return mult * pow_rem;
}
