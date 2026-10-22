/*
 * A322131  In the decimal expansion of n, replace each digit d with 2*d.
 * https://oeis.org/A322131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = concatenation of decimal strings of 2*d for each digit d
 *            of n
 * Range:     u8 n <= 24, u16 n <= 454, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Extract the decimal digits of n into an array in reverse order. */
    int digits[32];          /* 32 is enough for any 64‑bit unsigned integer */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % 10);
        tmp /= 10;
    }

    /* Build the result by processing digits from most significant to least. */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        if (d <= 4) {
            res = res * 10 + (seq_t)(2 * d);
        } else {
            res = res * 100 + (seq_t)(2 * d);
        }
    }
    return res;
}
