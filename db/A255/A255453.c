/*
 * A255453  A255452(2^n-1).
 * https://oeis.org/A255453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A255453
 * Recurrence:  a(n) = 5*a(n-1) - 6*a(n-2) + 4*a(n-3) - 2   for n ≥ 3
 * Initial values:  a(0)=1,  a(1)=5,  a(2)=21
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial terms separately */
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)5;
    if (n == (idx_t)2)
        return (seq_t)21;

    /* Iterate the recurrence up to n */
    seq_t a0 = (seq_t)1;   /* a(n-3)  */
    seq_t a1 = (seq_t)5;   /* a(n-2)  */
    seq_t a2 = (seq_t)21;  /* a(n-1)  */
    seq_t an = 0;

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        an = (seq_t)5 * a2;
        an = an - (seq_t)6 * a1;
        an = an + (seq_t)4 * a0;
        an = an - (seq_t)2;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
