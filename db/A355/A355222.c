/*
 * A355222  The k-th leftmost digit of a(n) is the greatest of the k leftmost
 *          digits of n.
 * https://oeis.org/A355222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A355222: a(n) is obtained from the decimal digits of n
   by replacing each digit with the maximum of all preceding digits
   (including itself). Leading zeros are ignored. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Extract decimal digits into an array, least significant first. */
    uint8_t digits[20];
    int len = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    seq_t result   = 0;
    seq_t cur_max  = 0;
    for (int i = len - 1; i >= 0; --i) {
        seq_t d = (seq_t)digits[i];
        if (d > cur_max) {
            cur_max = d;
        }
        result = result * 10 + cur_max;
    }

    return result;
}
