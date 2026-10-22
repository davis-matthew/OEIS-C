/*
 * A378380  Semiperimeter of the unique primitive Pythagorean triple whose
 *          inradius is A002315(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A378380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (r+1)*(2r+1) where r is the n-th term of the auxiliary
 *            sequence A002315, starting with r0 = 1, r1 = 7 and satisfying
 *            the linear recurrence r_{k+1} = 6*r_k – r_{k-1}
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 6;
    if (n == 1) return 120;
    seq_t r_prev = 1;   /* r0 */
    seq_t r_curr = 7;   /* r1 */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t r_next = 6 * r_curr - r_prev;
        r_prev = r_curr;
        r_curr = r_next;
    }
    return (r_curr + 1) * (2 * r_curr + 1);
}
