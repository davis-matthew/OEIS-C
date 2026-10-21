/*
 * A246884  Expansion of Sum_{n>=0} x^n * Sum_{k=0..n} C(n,k)^2 * x^(4*k).
 * https://oeis.org/A246884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..⌊n/5⌋} C(n-4k, k)^2
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 35, u64 n <= 84, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t kmax = n / 5;
    seq_t sum = 0;
    seq_t b   = 1;  /* b_k = C(n-4k, k) */
    for (idx_t k = 0; k <= kmax; ++k) {
        /* sum += b^2 */
        sum += b * b;

        if (k == kmax) break;   /* avoid computing b_{k+1} after last step */

        /* compute numerator = (n-5k)*(n-5k-1)*...*(n-5k-4) */
        seq_t num = (seq_t)(n - 5 * k) * (seq_t)(n - 5 * k - 1) *
                    (seq_t)(n - 5 * k - 2) * (seq_t)(n - 5 * k - 3) *
                    (seq_t)(n - 5 * k - 4);

        /* compute denominator = (k+1)*(n-4k)*(n-4k-1)*(n-4k-2)*(n-4k-3) */
        seq_t den = (seq_t)(k + 1) * (seq_t)(n - 4 * k) *
                    (seq_t)(n - 4 * k - 1) * (seq_t)(n - 4 * k - 2) *
                    (seq_t)(n - 4 * k - 3);

        /* b_{k+1} = b_k * num / den (exact division) */
        seq_t temp = b * num;
        b = temp / den;
    }
    return sum;
}
