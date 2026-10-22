/*
 * A394279  Number of induced cubes in the n X n black bishop graph.
 * https://oeis.org/A394279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(3 - 3*(-1)^n + n*(n + 1)*(n + 2))/24
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n must be at least 1 according to offset; return 0 for n==0 as safe guard */
    if (n == 0) return 0;

    seq_t n_t = (seq_t)n;
    seq_t n1 = n_t + 1;          /* n + 1 */
    seq_t n2 = n_t + 2;          /* n + 2 */

    /* term = n * (n+1) * (n+2) */
    seq_t term = n_t * n1 * n2;

    seq_t base;
    if ((n & 1) == 0) {          /* even n: (-1)^n = 1 → 3 - 3*1 = 0 */
        base = term;             /* even case: term alone */
    } else {                    /* odd n: (-1)^n = -1 → 3 - 3*(-1) = 6 */
        base = 6 + term;         /* odd case: 6 + term */
    }

    seq_t num = n1 * base;        /* numerator */
    return num / 24;              /* exact division – always integer */
}
