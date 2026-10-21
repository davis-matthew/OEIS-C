/*
 * A247386  Sum of the major index over all standard Young tableaux with n
 *          cells.
 * https://oeis.org/A247386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(n) * n * (n-1) / 4 * where f(n) is the number of
 *            standard Young tableaux * of size n. It satisfies f(0)=f(1)=1
 *            and * f(n)=f(n-1)+(n-1)*f(n-2) (the recurrence for involutions)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0)=0, a(1)=0 */
    if (n <= 1) return 0;

    /* compute f(n) by the recurrence */
    seq_t f_nm2 = 1;   /* f(0) */
    seq_t f_nm1 = 1;   /* f(1) */
    idx_t i;
    for (i = 2; i <= n; ++i) {
        seq_t temp = f_nm1 + (seq_t)(i - 1) * f_nm2;
        f_nm2 = f_nm1;
        f_nm1 = temp;
    }

    /* a(n) = f(n) * n * (n-1) / 4 */
    seq_t res = f_nm1;
    res *= (seq_t)n;
    res *= (seq_t)(n - 1);
    res /= (seq_t)4;

    return res;
}
