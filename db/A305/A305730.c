/*
 * A305730  a(n) is the total displacement of all letters in all permutations
 *          of n letters with no fixed points.
 * https://oeis.org/A305730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)*D(n)/3 where D(n) is the subfactorial
 *            (derangement) of n. a(0) = 0, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* D(0) = 1, D(1) = 0 */
    seq_t der_prev_prev = (seq_t)1;  /* D(i-2)   */
    seq_t der_prev      = (seq_t)0;  /* D(i-1)   */
    seq_t der_curr;

    for (idx_t i = 2; i <= n; ++i) {
        /* D(i) = (i-1) * (D(i-1) + D(i-2)) */
        seq_t sum = der_prev + der_prev_prev;
        der_curr = sum * (seq_t)(i - 1);
        der_prev_prev = der_prev;
        der_prev = der_curr;
    }

    /* Now der_prev holds D(n) */
    seq_t der_n = der_prev;
    return ((seq_t)n * (seq_t)(n + 1) * der_n) / (seq_t)3;
}
