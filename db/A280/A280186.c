/*
 * A280186  Number of 3-element subsets of S = {1..n} whose sum is odd.
 * https://oeis.org/A280186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)/12 for even n * a(n) = (n-1)*(n+1)*(n-3)/12
 *            for odd n * a(0)=a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 15, u16 n <= 93, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 4) return 0;
    if ((n & 1) == 0) {                 /* even n */
        seq_t tn0 = (seq_t)n;
        seq_t tn1 = (seq_t)(n - 1);
        seq_t tn2 = (seq_t)(n - 2);
        return tn0 * tn1 * tn2 / 12;
    } else {                          /* odd n */
        seq_t tn0 = (seq_t)(n - 1);
        seq_t tn1 = (seq_t)(n + 1);
        seq_t tn2 = (seq_t)(n - 3);
        return tn0 * tn1 * tn2 / 12;
    }
}
