/*
 * A381721  Sum of the legs of the unique primitive Pythagorean triple whose
 *          inradius is A000032(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A381721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(Lucas(n))^2 + 4*Lucas(n) + 1 Lucas numbers: L(0)=2,
 *            L(1)=1, L(n)=L(n-1)+L(n-2)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t lucas;       /* Lucas(n) */
    if (n == 0) {
        lucas = (seq_t)2;          /* L(0) */
    } else if (n == 1) {
        lucas = (seq_t)1;          /* L(1) */
    } else {
        seq_t prev = (seq_t)2;     /* L(0) */
        seq_t curr = (seq_t)1;     /* L(1) */
        for (idx_t i = 2; i <= n; ++i) {
            seq_t next = prev + curr;   /* L(i) = L(i-1)+L(i-2) */
            prev = curr;
            curr = next;
        }
        lucas = curr;                 /* L(n) */
    }

    /* a(n) = 2*lucas^2 + 4*lucas + 1 */
    seq_t res = (seq_t)2 * lucas * lucas
              + (seq_t)4 * lucas
              + (seq_t)1;
    return res;
}
