/*
 * A297242  Total variation of base-14 digits of n; see Comments.
 * https://oeis.org/A297242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of absolute differences between consecutive base‑14
 *            digits of n (least significant first)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 14)
        return 0;
    idx_t temp = n;
    idx_t last = temp % 14;   /* least significant digit */
    temp /= 14;
    seq_t total = 0;
    while (temp > 0) {
        idx_t cur = temp % 14;             /* next more significant digit */
        total += (last > cur) ? (last - cur) : (cur - last);
        last = cur;
        temp /= 14;
    }
    return total;
}
