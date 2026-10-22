/*
 * A373108  Number of edges among all distinct circles that can be constructed
 *          from the 4 vertices and the equally spaced 4*n points placed on
 *          the sides of a square when every pair of the 4 + 4*n points are
 *          connected by a circle and where the points lie at the ends of the
 *          circle's diameter.
 * https://oeis.org/A373108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = sum_{k=0}^{10} d_k * C(n, k) where the forward
 *            differences at 0 are d = [16, 180, 1232, 1532, 2592, -4216,
 *            14620, -48372, 143864, -388316, 986436]
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Forward difference coefficients */
    static const int64_t coeff[11] = {
        16,          /* C(n,0) */
        180,         /* C(n,1) */
        1232,        /* C(n,2) */
        1532,        /* C(n,3) */
        2592,        /* C(n,4) */
        -4216,       /* C(n,5) */
        14620,       /* C(n,6) */
        -48372,      /* C(n,7) */
        143864,      /* C(n,8) */
        -388316,     /* C(n,9) */
        986436       /* C(n,10) */
    };

    /* Use signed 128‑bit integer for intermediate sums and binomials
       (the final result fits in seq_t). */
    __int128 sum = 0;
    __int128 bin = 1;               /* C(n,0) */

    for (int k = 0; k <= 10; ++k) {
        sum += (__int128)coeff[k] * bin;

        if (k == 10)                /* finished all needed terms */
            break;

        /* If n <= k, all higher binomials are zero */
        if (n <= (idx_t)k) {
            bin = 0;
            continue;
        }

        /* C(n, k+1) = C(n, k) * (n - k) / (k + 1) */
        bin = (bin * (__int128)(n - (idx_t)k)) / (k + 1);
    }

    return (seq_t)sum;
}
