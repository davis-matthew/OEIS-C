/*
 * A372240  a(n) = Product_{j=1..n} j^(floor(sqrt(j))).
 * https://oeis.org/A372240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{j=1..n} j^(floor(sqrt(j)))
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_u64(idx_t n) {
    idx_t low = 0;
    idx_t high = n;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid == 0) {
            ans = 0;
            low = 1;
            continue;
        }
        if (mid > n / mid) {
            high = mid - 1;
        } else {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t j = 1; j <= n; ++j) {
        idx_t e = isqrt_u64(j);
        seq_t j_seq = (seq_t)j;
        for (idx_t k = 0; k < e; ++k) {
            result *= j_seq;
        }
    }
    return result;
}
