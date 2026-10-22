/*
 * A332498  a(n) = y(w+1) where y(0) = 0 and y(k+1) = 2^(k+1)-1-y(k) (resp.
 *          y(k)) when d_k = 2 (resp. d_k <> 2) and Sum_{k=0..w} d_k*3^k is
 *          the ternary representation of n. Sequence A332497 gives
 *          corresponding x's.
 * https://oeis.org/A332498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = y(w+1) where y(0)=0; For each ternary digit d_k of n
 *            (least‑significant first), if d_k==2: y(k+1) = 2^(k+1) - 1 -
 *            y(k); else y(k+1) = y(k). The function returns y after all
 *            digits are processed
 * Range:     u8 n <= 6560, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t y = 0;        /* current value of y   */
    seq_t pow2 = 1;     /* 2^k                 */
    idx_t tmp = n;

    while (1) {
        int d = tmp % 3;               /* digit d_k            */
        if (d == 2) {
            y = (pow2 * 2) - 1 - y;    /* reflection step      */
        }
        if (tmp < 3) break;            /* last digit processed */
        tmp /= 3;                      /* move to next digit   */
        pow2 *= 2;                     /* advance 2^(k+1)      */
    }
    return y;
}
