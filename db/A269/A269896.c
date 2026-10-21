/*
 * A269896  Number of n X 2 0..6 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling six exactly
 *          once.
 * https://oeis.org/A269896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 840 * (23*n - 15) * 31^(n-3) for n>1, a(1)=7
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)840;
    /* n >= 3 */
    idx_t e = n - 3;
    seq_t pow = (seq_t)1;
    seq_t base = (seq_t)31;
    while (e > 0) {
        if (e & 1) pow = pow * base;
        base = base * base;
        e >>= 1;
    }
    seq_t mult = ((seq_t)23 * (seq_t)n) - (seq_t)15;
    seq_t res = (seq_t)840 * mult;
    res = res * pow;
    return res;
}
