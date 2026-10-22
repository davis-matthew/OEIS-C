/*
 * A386015  Number of parking functions of size n with a descent in the first
 *          position.
 * https://oeis.org/A386015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/2) * (n+1)^(n-2) for n >= 2, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 0 */
    if (n < 2)
        return (seq_t)0;

    /* compute (n+1)^(n-2) by repeated multiplication */
    seq_t base = (seq_t)(n + (idx_t)1);
    seq_t power = (seq_t)1;
    for (idx_t i = 1; i <= n - 2; ++i)
        power *= base;

    /* multiply by n/2 (handle even/odd n) */
    if ((n & (idx_t)1) == (idx_t)0)     /* n even */
        return ((seq_t)(n / 2)) * power;
    else                                /* n odd */
        return ((seq_t)n * power) / ((seq_t)2);
}
