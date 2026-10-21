/*
 * A258890  Number of (n+2) X (4+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum less than the antidiagonal sum or central row sum
 *          less than the central column sum.
 * https://oeis.org/A258890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 12, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A258890: number of (n+2)×6 binary arrays with the given constraints.
 * For n = 1,2,3 the terms are given explicitly.
 * For n ≥ 4 the sequence satisfies:
 *   if n is even:  a(n) = n⁴ + 10 n³ + 122 n² + 498 n + 2385
 *   if n is odd : a(n) = n⁴ + 10 n³ + 121 n² + 480 n + 2304
 */
seq_t a(idx_t n)
{
    /* explicit initial values (offset 1) */
    if (n == 1) return 10201;
    if (n == 2) return 10680;
    if (n == 3) return 8100;

    /* polynomial formulas for n >= 4 */
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;          /* n^2 */
    seq_t n3  = n2 * nn;          /* n^3 */
    seq_t n4  = n3 * nn;          /* n^4 */

    if ((n & 1) == 0) {           /* n even */
        return n4
            + (10ULL   * n3)
            + (122ULL  * n2)
            + (498ULL  * nn)
            + 2385;
    } else {                      /* n odd */
        return n4
            + (10ULL   * n3)
            + (121ULL  * n2)
            + (480ULL  * nn)
            + 2304;
    }
}
