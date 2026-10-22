/*
 * A378386  Area of the unique primitive Pythagorean triple whose inradius is
 *          A002315(n) and such that its long leg and its hypotenuse are
 *          consecutive natural numbers.
 * https://oeis.org/A378386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002315(n) * (A002315(n)+1) * (2*A002315(n)+1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A002315(n) follows the recurrence R_0 = 1, R_1 = 7, R_{k+2} = 6*R_{k+1} - R_k */

seq_t a(idx_t n)
{
    /* compute the inradius R = A002315(n) */
    seq_t R;
    if (n == 0) {
        R = (seq_t)1;
    } else if (n == 1) {
        R = (seq_t)7;
    } else {
        seq_t R_prev = (seq_t)1;   /* R_{k-1}  */
        seq_t R_curr = (seq_t)7;   /* R_{k}    */
        for (idx_t k = 2; k <= n; ++k) {
            seq_t R_next = (seq_t)6 * R_curr - R_prev;
            R_prev = R_curr;
            R_curr = R_next;
        }
        R = R_curr;
    }

    /* compute the area = R * (R+1) * (2*R+1) */
    seq_t area = R * (R + (seq_t)1) * (((seq_t)2 * R) + (seq_t)1);
    return area;
}
