/*
 * A280970  Number of comparisons required to sort all permutations of [n] by
 *          MTF sort.
 * https://oeis.org/A280970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0 a(1) = 0 a(n) = a(n-1)*n + (n-1)! * (2^n + (n-3)*n/2)
 *            for n > 1 This function computes the sequence by iterating the
 *            recurrence while maintaining the factorial (n-1)! incrementally
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t a_val = (seq_t)0;
    seq_t fact   = (seq_t)1;          /* (i-1)! during the loop */
    seq_t pow2_val = (seq_t)4;        /* 2^2 for the first iteration */

    for (idx_t i = 2; i <= n; ++i) {
        /* Update factorial: (i-1)! */
        fact = fact * (seq_t)(i - 1);

        /* Compute B(i) = 2^i + (i-3)*i/2, handling the i==2 case separately */
        seq_t B;
        if (i == 2) {
            B = (seq_t)3;
        } else {
            B = pow2_val + ((seq_t)(i - 3) * (seq_t)i) / (seq_t)2;
        }

        /* term = (i-1)! * B(i) */
        seq_t term = fact * B;

        /* a(i) = a(i-1)*i + term */
        a_val = a_val * (seq_t)i + term;

        /* Prepare pow2_val for next iteration: 2^(i+1) */
        pow2_val = pow2_val * (seq_t)2;
    }

    return a_val;
}
