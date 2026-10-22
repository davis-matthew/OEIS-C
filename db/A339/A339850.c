/*
 * A339850  Number of Hamiltonian circuits within parallelograms of size 3 X n
 *          on the triangular lattice.
 * https://oeis.org/A339850
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) + 2*a(n-3) for n > 4 Initial values
 *            (offset = 2): a(2)=1, a(3)=4, a(4)=13
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* behaviour for n < 2 is unspecified; return 0 for safety */
    if (n < 2)
        return (seq_t)0;

    /* return initial values directly */
    if (n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)4;
    if (n == 4)
        return (seq_t)13;

    /* Compute recurrence iteratively, keeping only the last three terms.
       The values a2, a3, a4 correspond to a(n-3), a(n-2), a(n-1).   */
    seq_t a2 = (seq_t)1;   /* a(2) */
    seq_t a3 = (seq_t)4;   /* a(3) */
    seq_t a4 = (seq_t)13;  /* a(4) */
    seq_t ai = 0;

    for (idx_t i = 5; i <= n; ++i) {
        ai = (seq_t)2 * a4 + (seq_t)4 * a3 + (seq_t)2 * a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }

    return a4;
}
