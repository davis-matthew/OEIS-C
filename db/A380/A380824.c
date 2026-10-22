/*
 * A380824  Area of the unique primitive Pythagorean triple whose inradius is
 *          A000032(n) and such that its long leg and its hypotenuse are
 *          consecutive natural numbers.
 * https://oeis.org/A380824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n)*(Lucas(n) + 1)*(2*Lucas(n) + 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Lucas(0)=2, Lucas(1)=1 and Lucas(k)=Lucas(k-1)+Lucas(k-2)   */
seq_t a(idx_t n)
{
    seq_t L;
    if (n == 0) {
        L = 2;
    } else if (n == 1) {
        L = 1;
    } else {
        seq_t L_prev = 2;
        seq_t L_curr = 1;
        for (idx_t i = 2; i <= n; ++i) {
            seq_t L_next = L_prev + L_curr;
            L_prev = L_curr;
            L_curr = L_next;
        }
        L = L_curr;
    }
    /* area = L * (L + 1) * (2*L + 1)  */
    return L * (L + 1) * (L * 2 + 1);
}
