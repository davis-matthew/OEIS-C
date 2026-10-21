/*
 * A259109  2*A000540.
 * https://oeis.org/A259109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 7*n^3 + 21*n^5 + 21*n^6 + 6*n^7) / 21
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 27, u64 n <= 675, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first few terms are precomputed for speed and to avoid overflow */
    static const seq_t init[7] = {
        0ULL,     /* a(0) */
        2ULL,     /* a(1) */
        130ULL,   /* a(2) */
        1588ULL,  /* a(3) */
        9780ULL,  /* a(4) */
        41030ULL, /* a(5) */
        134342ULL /* a(6) */
    };

    /* terms 0..6 are returned directly */
    if (n < 7U)
        return init[n];

    /* for n >= 7, use the recurrence
       a(n) = a(n-1) + 2 * n^6
     */
    seq_t ans = init[6];           /* a(6) */
    for (idx_t k = 7U; k <= n; ++k)
    {
        seq_t kk = (seq_t)k;
        seq_t k2 = kk * kk;              /* k^2   */
        seq_t k3 = k2 * kk;              /* k^3   */
        seq_t k6 = k3 * k3;              /* k^6   */
        ans += (2 * k6);
    }
    return ans;
}
