/*
 * A277865  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 3", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A277865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n for even n * a(n) = 10^n + (10^{n-1} - 1)/9 for odd
 *            n * * Here 10^k denotes the decimal number consisting of a 1
 *            followed by k zeros. * For odd n the binary pattern is “10”
 *            followed by (n–1) ones, which * translates to the decimal number
 *            10^n + (10^{n-1}−1)/9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t pow_n = pow10(n);

    /* Even n: just 10^n. */
    if ((n & 1ULL) == 0)
        return pow_n;

    /* Odd n: 10^n + (10^{n-1}−1)/9. */
    seq_t pow_nm1 = pow10(n - 1);
    seq_t tmp = pow_nm1;
    tmp -= (seq_t)1;
    tmp /= (seq_t)9;
    return pow_n + tmp;
}
