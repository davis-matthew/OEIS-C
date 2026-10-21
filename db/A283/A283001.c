/*
 * A283001  a(n) = (A004186(n) - n)/9.
 * https://oeis.org/A283001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = (A004186(n) - n)/9
 * Range:     u8 n <= 356, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned char cnt[10] = {0};
    idx_t temp = n;

    /* Count digits of n */
    if (temp == 0) {
        cnt[0] = 1;
    } else {
        while (temp > 0) {
            cnt[temp % 10] += 1;
            temp /= 10;
        }
    }

    /* Build the number with digits sorted in non‑increasing order */
    seq_t sorted = 0;
    for (int d = 9; d >= 0; --d) {
        for (int k = 0; k < cnt[d]; ++k) {
            sorted = sorted * 10 + (seq_t)d;
        }
    }

    /* Compute the sequence value */
    return (sorted - (seq_t)n) / (seq_t)9;
}
