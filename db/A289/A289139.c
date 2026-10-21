/*
 * A289139  a(n) is the number of odd integers divisible by 7 in ]4*(n-1)^2,
 *          4*n^2[.
 * https://oeis.org/A289139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 447, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+7) = a(n) + 4,
   initial values:
   a(0)=0, a(1)=0, a(2)=1, a(3)=2, a(4)=2, a(5)=2, a(6)=3 */
seq_t a(idx_t n) {
    /* quotient and remainder of n divided by 7 */
    idx_t q = n / 7;
    idx_t r = n % 7;
    /* first 7 values repeated with linear offset 4 per 7 steps */
    static const seq_t lut[7] = { (seq_t)0, (seq_t)0, (seq_t)1,
                                  (seq_t)2, (seq_t)2, (seq_t)2,
                                  (seq_t)3 };
    return lut[r] + (seq_t)4 * (seq_t)q;
}
