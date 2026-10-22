/*
 * A317790  a(n) = 2*a(n-1) - a(n-2) + a(n-4) - 2*(n-5) + a(n-6) for n>5,
 *          a(0)=a(1)=1, a(2)=a(3)=7, a(4)=13, a(5)=19.
 * https://oeis.org/A317790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^2 + 3*(3 - 2*(-1)^(n/2))*(1 + (-1)^n) + 2)/8
 * Range:     u8 n <= 18, u16 n <= 295, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_t = (seq_t)n;
    if (n_t & 1) {                           /* n odd: (-1)^n = -1, 1+(-1)^n = 0 */
        return ((seq_t)6 * n_t * n_t + (seq_t)2) / (seq_t)8;
    } else {                                 /* n even */
        seq_t half = n_t / 2;
        if (half & 1) {                      /* half odd: (-1)^(n/2) = -1 */
            return ((seq_t)6 * n_t * n_t + (seq_t)32) / (seq_t)8;
        } else {                            /* half even: (-1)^(n/2) = +1 */
            return ((seq_t)6 * n_t * n_t + (seq_t)8) / (seq_t)8;
        }
    }
}
