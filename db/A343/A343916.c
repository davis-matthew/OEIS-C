/*
 * A343916  a(n) is the minimal total number of faces of a polyhedron with at
 *          least one i-gonal face for every i in 3..n.
 * https://oeis.org/A343916
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 516, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A343916
 * a(n) = ceil((n^2 - 11*n + 62)/4) for n ≥ 14
 * a(3) = 4, a(4) = 5, a(5) = 6, a(6) = 7, a(7) = 8, a(8) = 10,
 * a(9) = 11, a(10) = 14, a(11) = 16, a(12) = 19, a(13) = 23  */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;                     /* undefined for n < 3 */

    /* precomputed values for 3 ≤ n ≤ 13 */
    if (n < 14) {
        /* index: n-3  values from OEIS list */
        static const seq_t base[] = {
            (seq_t)4, /* n=3 */
            (seq_t)5, /* n=4 */
            (seq_t)6, /* n=5 */
            (seq_t)7, /* n=6 */
            (seq_t)8, /* n=7 */
            (seq_t)10,/* n=8 */
            (seq_t)11,/* n=9 */
            (seq_t)14,/* n=10 */
            (seq_t)16,/* n=11 */
            (seq_t)19,/* n=12 */
            (seq_t)23 /* n=13 */
        };
        return base[n - 3];
    }

    /* n ≥ 14: compute ceil((n^2 - 11*n + 62)/4) */
    seq_t nn = (seq_t)n;
    seq_t num = nn * nn - (seq_t)11 * nn + (seq_t)62;
    seq_t res = num / (seq_t)4;
    if (num % (seq_t)4) res++;               /* add one if remainder != 0 */
    return res;
}
