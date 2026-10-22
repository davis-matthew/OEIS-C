/*
 * A391167  Face-centered cubic pyritohedral numbers: a(n) = 3456*n^3 -
 *          8424*n^2 + 6848*n - 1854.
 * https://oeis.org/A391167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3456*n^3 - 8424*n^2 + 6848*n - 1854
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 108, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    /* Compute the polynomial term by term, casting n to seq_t first */
    seq_t term = 3456 * x * x * x;   /* 3456*n^3 */
    term -= 8424 * x * x;            /*  - 8424*n^2 */
    term += 6848 * x;                /*  + 6848*n   */
    term -= 1854;                    /*  - 1854     */
    return term;
}
