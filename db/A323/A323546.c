/*
 * A323546  a(n) = Product_{k=0..n} (k^8 + (n-k)^8).
 * https://oeis.org/A323546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^8 + (n-k)^8)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* special case n = 0, the product contains 0^8 + 0^8 = 0 */
    if (n == 0)
        return 0;

    seq_t result = 1;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t a = (seq_t)k;
        seq_t b = (seq_t)(n - k);

        /* compute a^8 */
        seq_t a_pow = a;
        a_pow *= a; /* a^2 */
        a_pow *= a; /* a^3 */
        a_pow *= a; /* a^4 */
        a_pow *= a; /* a^5 */
        a_pow *= a; /* a^6 */
        a_pow *= a; /* a^7 */
        a_pow *= a; /* a^8 */

        /* compute b^8 */
        seq_t b_pow = b;
        b_pow *= b;
        b_pow *= b;
        b_pow *= b;
        b_pow *= b;
        b_pow *= b;
        b_pow *= b;
        b_pow *= b;

        seq_t term = a_pow + b_pow;
        result *= term;
    }
    return result;
}
