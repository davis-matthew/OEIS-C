/*
 * A386649  Product of first n central trinomial coefficients (A002426) for n
 *          > 0 with a(0) = 1.
 * https://oeis.org/A386649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=0..n} A002426(k) where A002426(0)=1, A002426(1)=1,
 *            and for k≥2: k*A_k = (2k-1)A_{k-1} + (3k-3)A_{k-2}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* C0 (k=0) and C1 (k=1) */
    seq_t c_prev2 = (seq_t)1; /* A002426(0) */
    seq_t c_prev1 = (seq_t)1; /* A002426(1) */

    seq_t product = (seq_t)1; /* product so far, initially 1 */
    product *= c_prev1;       /* multiply by A002426(1) */

    /* Compute for k = 2 .. n */
    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t term1 = ((seq_t)(2*k - 1)) * c_prev1;
        seq_t term2 = ((seq_t)(3*k - 3)) * c_prev2;
        seq_t c_cur = (term1 + term2) / k;

        product *= c_cur;

        /* Shift for next iteration */
        c_prev2 = c_prev1;
        c_prev1 = c_cur;
    }

    return product;
}
