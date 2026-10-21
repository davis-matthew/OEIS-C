/*
 * A256307  Apply the transformation 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 0 to
 *          the digits of n written in base 7; do not convert back to base 10.
 * https://oeis.org/A256307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal number obtained from n by writing it in base‑7,
 *            adding 1 mod 7 to each digit, and interpreting the resulting
 *            digit string as a decimal integer
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    unsigned char digits[64];   /* holds base‑7 digits (least significant first) */
    int cnt = 0;
    idx_t temp = n;

    if (temp == 0) {            /* special case */
        digits[cnt++] = 0;
    } else {
        while (temp) {
            digits[cnt++] = temp % 7;
            temp /= 7;
        }
    }

    seq_t res = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        unsigned char d = digits[i];
        unsigned char nd = d + 1;
        if (nd == 7) nd = 0;    /* (d + 1) mod 7 */
        res = res * 10 + nd;
    }
    return res;
}
