/*
 * A308235  Take apart the sides of each of the integer-sided scalene
 *          triangles with perimeter n (at their vertices) and rearrange them
 *          orthogonally in 3-space so that their endpoints coincide at a
 *          single point. a(n) is the total surface area of all rectangular
 *          prisms enclosed in this way.
 * https://oeis.org/A308235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{k=1..⌊(n-1)/3⌋} Σ_{i=k+1..⌊(n-k-1)/2⌋}
 *            sign(⌊(i+k)/(n-i-k+1)⌋) * (i*k + i*(n-i-k) + k*(n-i-k)) where
 *            sign(x)=1 if x>0, 0 if x=0. (For integer‑sided scalene triangles
 *            with perimeter n.)
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n < 9) return 0;                     /* quick return for early zeros   */

    const idx_t bound = (n + 2) / 2;          /* ceil((n+1)/2)                     */
    const idx_t kmax  = (n - 1) / 3;          /* floor((n-1)/3)                     */

    for (idx_t k = 1; k <= kmax; ++k)
    {
        const idx_t i_min = k + 1;
        const idx_t i_max = (n - k - 1) / 2;
        if (i_min > i_max) continue;

        /* first index where the sign is non‑zero:
           need i+k >= bound  ->  i >= bound - k   */
        idx_t i_start = i_min;
        const idx_t req = (bound > k) ? (bound - k) : 0;
        if (i_start < req) i_start = req;
        if (i_start > i_max) continue;

        const seq_t sn = (seq_t)n;
        for (idx_t i = i_start; i <= i_max; ++i)
        {
            const seq_t ii = (seq_t)i;
            const seq_t kk = (seq_t)k;
            const seq_t term =
                sn * (ii + kk) - (ii * ii + kk * kk + ii * kk);
            res += 2 * term;
        }
    }
    return res;
}
