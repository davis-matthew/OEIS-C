/*
 * A349768  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+k,k) *
 *          binomial(2*k,k) / (k+1).
 * https://oeis.org/A349768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+k,k) *
 *            binomial(2*k,k) / (k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum      = 0;
    seq_t c_nk     = 1;      /* C(n,k)          */
    seq_t c_nk2    = 1;      /* C(n+k,k)        */
    seq_t c_2k_k   = 1;      /* C(2k,k)         */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            c_nk      = c_nk * (seq_t)(n - k + 1) / (seq_t)k;
            c_nk2     = c_nk2 * (seq_t)(n + k)     / (seq_t)k;
            c_2k_k    = c_2k_k * (seq_t)(2 * k)
                              * (seq_t)(2 * k - 1)
                      / ((seq_t)k * (seq_t)k);
        }
        seq_t term = c_nk * c_nk2 * c_2k_k / ((seq_t)(k + 1));
        sum += term;
    }
    return sum;
}
