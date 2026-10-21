/*
 * A266435  Binary representation of the n-th iteration of the "Rule 23"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(n) = ((-1)^n-1)*(1-10^(2n+1))/18,
 *            equivalently a(n)=0 when n is even, a(n)=(10^(2n+1)-1)/9 when n
 *            is odd
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"    /* provides seq_t pow10(idx_t e) */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* even n (n>0) gives 0 */
    if ((n & 1ULL) == 0) {
        return (seq_t)0;
    }

    /* odd n: a(n) = (10^(2n+1)-1)/9 */
    idx_t exp = 2 * n + 1;
    seq_t ten_pow = pow10(exp);
    seq_t value   = (ten_pow - (seq_t)1) / (seq_t)9;
    return value;
}
