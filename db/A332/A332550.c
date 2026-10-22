/*
 * A332550  The 10-block imbalance of n (see Comments for definition).
 * https://oeis.org/A332550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10 * max_digit_frequency(n) - digits_in_n(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t orig = n;
    unsigned char freq[10] = {0};
    idx_t len = 0;
    idx_t tmp = orig;

    if (tmp == 0) {
        freq[0] = 1;
        len = 1;
    } else {
        while (tmp > 0) {
            unsigned d = tmp % 10;
            freq[d] += 1;
            tmp /= 10;
            len += 1;
        }
    }

    unsigned char maxfreq = 0;
    for (int d = 0; d < 10; ++d) {
        if (freq[d] > maxfreq) {
            maxfreq = freq[d];
        }
    }

    seq_t result = (seq_t)10 * (seq_t)maxfreq - (seq_t)len;
    return result;
}
