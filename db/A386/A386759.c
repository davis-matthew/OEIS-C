/*
 * A386759  a(n) = 4^n * (4*n)! / (n! * (4!)^n).
 * https://oeis.org/A386759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * (4*n)! / (n! * (4!)^n) Recurrence: a(0)=1,
 *            a(n)=a(n-1)*4*(4*n-3)*(2*n-1)*(4*n-1)/3 for n≥1 (The factor is
 *            always divisible by 3.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t factor = (seq_t)4;
        factor *= (seq_t)(4 * i - 3);
        factor *= (seq_t)(2 * i - 1);
        factor *= (seq_t)(4 * i - 1);
        factor /= (seq_t)3;   /* exact division */
        result *= factor;
    }
    return result;
}
