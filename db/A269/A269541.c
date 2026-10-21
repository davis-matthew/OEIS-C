/*
 * A269541  Number of length-7 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than one.
 * https://oeis.org/A269541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^7 + 7*n^6 + 6*n^5 + 20*n^4 - 12*n^3 + 22*n^2 - 18*n + 4
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n ≥ 1.  For n = 0 the function returns 0.
 * All intermediate values are computed in seq_t; the build supplies a
 * sufficiently large integer type (unsigned 8,16,32,64 or an arbitrary‑precision
 * class).
 */

#include <stdint.h>   /* for uintptr_t in the build – harmless */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t N  = (seq_t)n;          /* n */
    seq_t N2 = N * N;             /* n^2 */
    seq_t N3 = N2 * N;            /* n^3 */
    seq_t N4 = N3 * N;            /* n^4 */
    seq_t N5 = N4 * N;            /* n^5 */
    seq_t N6 = N5 * N;            /* n^6 */
    seq_t N7 = N6 * N;            /* n^7 */

    seq_t res = N7;                       /* n^7 */
    res += (seq_t)7 * N6;                 /* + 7 n^6 */
    res += (seq_t)6 * N5;                 /* + 6 n^5 */
    res += (seq_t)20 * N4;                /* + 20 n^4 */
    res -= (seq_t)12 * N3;                /* - 12 n^3 */
    res += (seq_t)22 * N2;                /* + 22 n^2 */
    res -= (seq_t)18 * N;                 /* - 18 n   */
    res += (seq_t)4;                      /* + 4      */

    return res;
}
