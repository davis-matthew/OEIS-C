/*
 * A297043  Number of pieces in the list d(m), d(m-1), ..., d(0) of base-15
 *          digits of n; see Comments.
 * https://oeis.org/A297043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of maximal monotone runs in the base‑15 digit
 *            representation of n. Each run consists of consecutive digits
 *            that are either strictly increasing, strictly decreasing or
 *            equal. The value equals the number of runs, which is 0 for n<15
 *            (single‑digit base‑15)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 15) return 0;          /* single digit → no pieces */

    unsigned char digits[20];       /* enough for 64‑bit numbers in base 15 */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 15);
        tmp /= 15;
    }

    int pieces = 0;
    int prev_sign = 0;              /* no previous sign yet */

    for (int i = len - 1; i > 0; --i) {
        int diff = (int)digits[i] - (int)digits[i - 1];
        int sign = (diff > 0) ? 1 : (diff < 0) ? -1 : 0;

        if (pieces == 0) {         /* first comparison starts first run */
            pieces = 1;
        } else if (sign != prev_sign) {
            ++pieces;              /* new run starts when sign changes */
        }
        prev_sign = sign;
    }

    return (seq_t)pieces;
}
