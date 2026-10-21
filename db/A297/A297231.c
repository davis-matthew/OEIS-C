/*
 * A297231  Down-variation of the base-11 digits of n; see Comments.
 * https://oeis.org/A297231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = down‑variation of the base‑11 digits of n a(n) = Σ max(0,
 *            d_i - d_{i-1}) over all adjacent digit pairs in base‑11
 *            representation
 * Range:     u8 n <= 1705, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t temp = (seq_t)n;
    /* Extract least significant base‑11 digit */
    seq_t prev = temp % 11;
    temp /= 11;
    while (temp != 0) {
        seq_t curr = temp % 11;
        if (curr > prev)
            sum += (curr - prev);
        prev = curr;
        temp /= 11;
    }
    return sum;
}
