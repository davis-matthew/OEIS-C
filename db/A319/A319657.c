/*
 * A319657  Write n in 9-ary, sort digits into increasing order.
 * https://oeis.org/A319657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = the integer whose base‑9 digits are the base‑9 digits of
 *            n sorted in nondecreasing order (sorted ascending)
 * Range:     u8 n <= 272, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    unsigned char digits[80];
    idx_t len = 0;

    /* Extract base‑9 digits (least–significant first). */
    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 9);
        tmp /= 9;
    }

    /* Count occurrences of each digit (0 … 8). */
    unsigned char freq[9] = {0};
    for (idx_t i = 0; i < len; ++i)
        freq[digits[i]]++;

    /* Re‑construct the number using the digits in ascending order. */
    seq_t result = 0;
    for (int d = 0; d < 9; ++d) {
        while (freq[d]--) {
            result = result * 9 + d;
        }
    }

    return result;
}
