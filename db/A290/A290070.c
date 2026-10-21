/*
 * A290070  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 641", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-2) - 10*a(n-3) for n > 4; initial values:
 *            a(0)=1, a(1)=10, a(2)=101, a(3)=1100, a(4)=11101
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle the known initial segment explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)101;
    if (n == 3) return (seq_t)1100;
    if (n == 4) return (seq_t)11101;

    /* iterate the linear recurrence from n=5 up to the requested n */
    seq_t a_n_minus3 = (seq_t)101;     /* a(2)   */
    seq_t a_n_minus2 = (seq_t)1100;    /* a(3)   */
    seq_t a_n_minus1 = (seq_t)11101;   /* a(4)   */
    seq_t a_n = 0;

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 10*a(i-1) + a(i-2) - 10*a(i-3) */
        a_n = (seq_t)10 * a_n_minus1 + a_n_minus2 - (seq_t)10 * a_n_minus3;

        /* shift for next iteration */
        a_n_minus3 = a_n_minus2;
        a_n_minus2 = a_n_minus1;
        a_n_minus1 = a_n;
    }

    return a_n;
}
