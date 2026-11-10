/*
 * A391589  a(n) = A003817(n) - A115873(n), where A115873(n) = the least k >=
 *          1 for which A048720(A065621(n),k) = n*k.
 * https://oeis.org/A391589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A003817(n) - A115873(n), where A115873(n) is the least k
 *            >= 1 with A048720(A065621(n),k) = n*k; computed by searching k
 *            from 1 upward (k <= A003817(n) < 2n)
 * Range:     u8 n <= 16, u16 n <= 1527, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 100 terms through n = 9950 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A003817_a(idx_t n) {
    if (n == 0) return 0;
    seq_t result = 1;
    idx_t t = n;
    while (t > 1) {
        t >>= 1;
        result = result * 2 + 1;
    }
    return result;
}

static seq_t A065621_a(idx_t n) {
    seq_t n_t = (seq_t)n;
    seq_t low_t = (seq_t)(n & (~n + 1));
    return n_t ^ ((n_t - low_t) << 1);
}

static seq_t A048720_gf2(seq_t i, seq_t j) {
    seq_t r = 0;
    seq_t s = i;
    while (j > 0) {
        if ((j & 1) != 0) r ^= s;
        s <<= 1;
        j >>= 1;
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t top = A003817_a(n);
    seq_t m = A065621_a(n);
    seq_t nn = (seq_t)n;
    for (seq_t k = 1; k <= top; k++) {
        if (A048720_gf2(m, k) == nn * k) {
            return top - k;
        }
    }
    return 0;
}
