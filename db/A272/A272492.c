/*
 * A272492  Number of ordered set partitions of [n] with nondecreasing block
 *          sizes and maximal block size equal to two.
 * https://oeis.org/A272492
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - n!/2^{⌊n/2⌋}. Recurrence: 2*a(n) = 2*n*a(n-1) +
 *            (n-1)*n*a(n-2) - (n-2)*(n-1)*n*a(n-3), with initial values
 *            a(2)=1, a(3)=3, a(4)=18
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)2) return (seq_t)1;
    if (n == (idx_t)3) return (seq_t)3;
    if (n == (idx_t)4) return (seq_t)18;

    seq_t a0 = (seq_t)1;   /* a(2) */
    seq_t a1 = (seq_t)3;   /* a(3) */
    seq_t a2 = (seq_t)18;  /* a(4) */

    for (idx_t i = (idx_t)5; i <= n; ++i)
    {
        seq_t rhs = (seq_t)0;
        rhs += (seq_t)2 * (seq_t)i * a2;
        rhs += (seq_t)(i - (idx_t)1) * (seq_t)i * a1;
        rhs -= (seq_t)(i - (idx_t)2) * (seq_t)(i - (idx_t)1)
                * (seq_t)i * a0;

        seq_t an = rhs / (seq_t)2;

        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return a2;
}
