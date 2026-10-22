/*
 * A350043  Sum of all the parts > 1 in the partitions of n into 3 positive
 *          integer parts.
 * https://oeis.org/A350043
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Σ_{k=1}^{⌊n/3⌋} ⌊(n-3k+2)/2⌋ - 1 - ⌊(n-1)/2⌋ for n ≥
 *            4; a(3) = 0
 * Range:     u8 n <= 14, u16 n <= 92, u32 n <= 3721, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the offset (n=3 is the first defined term) */
    if (n <= 3) {
        return (seq_t)0;
    }

    /* Compute the summation part: Σ_{k=1}^{⌊n/3⌋} ⌊(n-3k+2)/2⌋ */
    idx_t kmax = n / 3;                 /* floor(n/3) */
    seq_t sum = 0;
    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t t = n - 3 * k + 2;        /* n - 3k + 2  (always ≥ 2 for k ≤ floor(n/3)) */
        sum += (seq_t)(t / 2);          /* floor division by 2 */
    }

    /* Assemble the final value */
    seq_t n_seq = (seq_t)n;
    seq_t result = n_seq * sum;
    result -= (seq_t)1;
    result -= (seq_t)((n - 1) / 2);     /* floor((n-1)/2) */

    return result;
}
