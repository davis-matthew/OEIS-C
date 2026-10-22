/*
 * A359761  a(n) = binomial(4*n, 2*n)*(2*n)!/(2^n*n!).
 * https://oeis.org/A359761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n,2*n)*(2*n)!/(2^n*n!) A closed‑form
 *            recurrence is a(0)=1 a(n) = a(n-1)*2*(4*n-1)*(4*n-3)/n (n≥1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t k = (idx_t)1; k <= n; ++k) {
        seq_t num = (seq_t)2 * ((seq_t)(4*k - 1u))
                        * ((seq_t)(4*k - 3u));
        res = res * num / (seq_t)k;
    }
    return res;
}
