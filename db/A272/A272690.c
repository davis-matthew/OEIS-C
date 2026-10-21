/*
 * A272690  a(n) = 22*Sum_{i=0..n-2} 46^i*2^(n-2-i) + 2^(n-1).
 * https://oeis.org/A272690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 48*a(n-1) - 92*a(n-2) (for n > 2) Initial values: a(1) =
 *            1, a(2) = 24 This implements OEIS sequence A272690
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not part of the sequence, safe fallback */
    if (n == 1) return 1;
    if (n == 2) return 24;

    seq_t a_prev2 = 1;             /* a(1) */
    seq_t a_prev1 = 24;            /* a(2) */
    seq_t a_curr = 0;

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = (seq_t)48 * a_prev1 - (seq_t)92 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
