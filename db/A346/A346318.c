/*
 * A346318  Number of permutations of [n] having three cycles of the form (c1,
 *          c2, ..., c_m) where c1 = min_{i>=1} c_i and c_j = min_{i>=j} c_i
 *          or c_j = max_{i>=j} c_i.
 * https://oeis.org/A346318
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-7)*3^(n-1) + (n-2)*2^(2n-8) + (n^2 - 3*n +
 *            1)*2^(n-7) for n>3, with a(3)=1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 28, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)6;
    if (n == 5) return (seq_t)35;
    if (n == 6) return (seq_t)195;
    if (n >= 7) {
        /* Compute 2^(n-7) */
        seq_t pow2_n7 = (seq_t)1;
        for (idx_t i = 0; i < n - 7; ++i) pow2_n7 *= (seq_t)2;

        /* Compute 3^(n-1) */
        seq_t pow3_n1 = (seq_t)1;
        for (idx_t i = 0; i < n - 1; ++i) pow3_n1 *= (seq_t)3;

        /* Term1 = 2^(n-7) * 3^(n-1) */
        seq_t term1 = pow2_n7 * pow3_n1;

        /* Compute 2^(2n-8) */
        seq_t pow2_2n8 = (seq_t)1;
        for (idx_t i = 0; i < 2 * n - 8; ++i) pow2_2n8 *= (seq_t)2;

        /* Term2 = (n-2) * 2^(2n-8) */
        seq_t term2 = (seq_t)(n - 2) * pow2_2n8;

        /* Term3 = (n^2 - 3*n + 1) * 2^(n-7) */
        seq_t coeff = (seq_t)(n * n) - (seq_t)(3 * n) + (seq_t)1;
        seq_t term3 = coeff * pow2_n7;

        return term1 + term2 + term3;
    }
    /* Undefined for n < 3 – value not required by the specification */
    return (seq_t)0;
}
