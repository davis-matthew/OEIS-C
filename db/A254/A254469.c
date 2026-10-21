/*
 * A254469  Sixth partial sums of cubes (A000578).
 * https://oeis.org/A254469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 37, u64 n <= 466, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A254469: Sixth partial sums of cubes
   Formula: a(n) = n*(n+1)^2*(n+2)*(n+3)*(n+4)*(n+5)^2*(n+6)/60480
   for n >= 1 (offset 1). */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset not used, but safe */

    /* Use an unsigned 128‑bit intermediate to avoid overflow for typical
     * ranges of n (up to about 10⁴).  For larger n the intermediate may
     * overflow, but such values are not checked in the 64‑bit build. */
    unsigned __int128 prod = (unsigned __int128)n;

    prod *= (unsigned __int128)(n + 1);   /* first (n+1)   */
    prod *= (unsigned __int128)(n + 1);   /* second (n+1)  */
    prod *= (unsigned __int128)(n + 2);
    prod *= (unsigned __int128)(n + 3);
    prod *= (unsigned __int128)(n + 4);
    prod *= (unsigned __int128)(n + 5);   /* first (n+5)   */
    prod *= (unsigned __int128)(n + 5);   /* second (n+5)  */
    prod *= (unsigned __int128)(n + 6);

    prod /= (unsigned __int128)60480;

    return (seq_t)prod;
}
