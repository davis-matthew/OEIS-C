/*
 * A319720  Write n in 4-ary, sort digits into decreasing order.
 * https://oeis.org/A319720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = base‑4 representation of n sorted with digits in
 *            decreasing order, then interpreted in base‑4
 * Range:     u8 n <= 255, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t cnt[4] = {0};
    idx_t temp = n;
    /* Count base‑4 digits (use bitwise operations only). */
    while (temp != 0) {
        cnt[temp & 3]++;   /* digit = temp % 4 via bit mask */
        temp >>= 2;        /* divide by 4 */
    }
    /* Reconstruct the number from digits sorted descending. */
    seq_t result = 0;
    for (int d = 3; d >= 0; --d) {
        for (idx_t i = 0; i < cnt[d]; ++i) {
            result = result * 4 + d;
        }
    }
    return result;
}
