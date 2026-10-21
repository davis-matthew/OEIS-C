/*
 * A250854  Number of (2+1) X (n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*n^6 + 141*n^5 + 953*n^4 + 3423*n^3 + 6527*n^2 + 6192*n
 *            + 2304) / 36 (n >= 1)
 * Range:     u8 none, u16 n <= 5, u32 n <= 48, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute with 128‑bit intermediate to avoid overflow on 64‑bit builds. */
    __int128 nn = n;
    __int128 val = 8;                 /* start with coefficient of n^6   */
    val = val * nn + 141;             /* now 8*n^2 + 141*n + 141       */
    val = val * nn + 953;
    val = val * nn + 3423;
    val = val * nn + 6527;
    val = val * nn + 6192;
    val = val * nn + 2304;
    val /= 36;
    return (seq_t)val;
}
