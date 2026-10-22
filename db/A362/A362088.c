/*
 * A362088  a(n) = Sum_{k=0..n} (-1)^k * binomial(-n,k) * binomial(3*k,n-k).
 * https://oeis.org/A362088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k-1,k) * binomial(3*k,n-k)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset 0  (a(0) = 1) */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Handle the trivial case n=0 */
    if (n == 0) return (seq_t)1;

    idx_t first_non_zero_k = (n + 3) / 4;          /* ceil(n/4) */

    seq_t sum = 0;
    seq_t A   = 1;                                 /* C(n+k-1, k)   */
    seq_t B   = 0;                                 /* C(3k, n-k)    */

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0) {                              /* k = 0 < first_non_zero_k */
            /* A is already 1 */
            B = 0;                                 /* C(0, n) = 0 for n>0 */
        } else if (k < first_non_zero_k) {
            /* B stays zero; only update A */
            A = (A * ((seq_t)n + k - 1)) / (seq_t)k;
        } else if (k == first_non_zero_k) {
            /* First non‑zero term of B */
            A = (A * ((seq_t)n + k - 1)) / (seq_t)k;
            B = binomial(3 * k, n - k);           /* compute once */
        } else {
            /* k > first_non_zero_k : update both A and B */
            A = (A * ((seq_t)n + k - 1)) / (seq_t)k;

            /* Ratio R_k = B_k / B_{k-1} */
            /* B_k = B_{k-1} * num / den  */
            seq_t num = (seq_t)(3 * k) *
                        (seq_t)(3 * k - 1) *
                        (seq_t)(3 * k - 2) *
                        (seq_t)(n - k + 1);

            seq_t den = (seq_t)(4 * k - n) *
                        (seq_t)(4 * k - n - 1) *
                        (seq_t)(4 * k - n - 2) *
                        (seq_t)(4 * k - n - 3);

            B = (B * num) / den;
        }
        sum += A * B;
    }
    return sum;
}
