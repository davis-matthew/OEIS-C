/*
 * A319804  a(n) = A319720(n) + A319652(n).
 * https://oeis.org/A319804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A319720(n) + A319652(n), where A319720 sorts base-4
 *            digits of n in decreasing order and A319652 sorts them in
 *            increasing order
 * Range:     u8 n <= 78, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Validated: standard, pass, 84 terms through n = 3609 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A319804_desc(idx_t n) {
    idx_t cnt[4] = {0, 0, 0, 0};
    idx_t t = n;
    while (t != 0) {
        cnt[t & 3]++;
        t >>= 2;
    }
    seq_t r = 0;
    for (int d = 3; d >= 0; --d) {
        for (idx_t i = 0; i < cnt[d]; ++i) {
            r = r * 4 + (seq_t)d;
        }
    }
    return r;
}

static seq_t A319804_asc(idx_t n) {
    idx_t cnt[4] = {0, 0, 0, 0};
    idx_t t = n;
    while (t != 0) {
        cnt[t & 3]++;
        t >>= 2;
    }
    seq_t r = 0;
    for (int d = 0; d <= 3; ++d) {
        for (idx_t i = 0; i < cnt[d]; ++i) {
            r = r * 4 + (seq_t)d;
        }
    }
    return r;
}

seq_t a(idx_t n) {
    return A319804_desc(n) + A319804_asc(n);
}
