/*
 * A264129  Number of (n+1) X (4+1) arrays of permutations of 0..n*5+4 with
 *          each element having index change +-(.,.) 0,0 0,2 or 1,2.
 * https://oeis.org/A264129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 25*a(n-2), a(1)=80, a(2)=1040
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)80;
    if (n == 2) return (seq_t)1040;

    seq_t f_nm2 = (seq_t)80;   /* a(n-2) */
    seq_t f_nm1 = (seq_t)1040; /* a(n-1) */
    seq_t result = f_nm1;

    for (idx_t i = 3; i <= n; ++i) {
        result = (seq_t)15 * f_nm1 - (seq_t)25 * f_nm2;
        f_nm2 = f_nm1;
        f_nm1 = result;
    }

    return result;
}
