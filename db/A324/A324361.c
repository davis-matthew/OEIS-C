/*
 * A324361  Total number of occurrences of n in the (signed) displacement sets
 *          of all permutations of [2n] divided by n!.
 * https://oeis.org/A324361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula implemented: a(n) = (8*n-12)*a(n‑1)
 *                                 - (16*n^2-64*n+59)*a(n‑2)
 *                                 - (4*n-10)*a(n‑3)
 *   with a(0)=0, a(1)=1, a(2)=5. */
seq_t a(idx_t n) {
    /* base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    /* a0  = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)0;          /* a(0)  */
    seq_t a1 = (seq_t)1;          /* a(1)  */
    seq_t a2 = (seq_t)5;          /* a(2)  */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t coeff1 = (seq_t)(8 * i - 12);
        seq_t coeff2 = (seq_t)(16 * i * i - 64 * i + 59);
        seq_t coeff3 = (seq_t)(4 * i - 10);

        seq_t ai = coeff1 * a2
                 - coeff2 * a1
                 - coeff3 * a0;

        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
