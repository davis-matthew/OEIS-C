/*
 * A394785  a(n) = number of triples (x, y, z) such that x^2 + y*z = n, where
 *          x,y,z are positive integers satisfying x^2 <= y*z.
 * https://oeis.org/A394785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1 <= x <= floor(sqrt(n/2))} d(n - x^2) where d(k) is
 *            the number of positive divisors of k
 * Range:     u8 n <= 840, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t n) {
    idx_t low = 0, high = 1ULL << 32;   /* high > sqrt(UINT64_MAX) */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

static seq_t divisor_count_u64(idx_t m) {
    seq_t cnt = 0;
    if (m == 0) return 0;  /* not used for valid m */
    idx_t root = isqrt_u64(m);
    for (idx_t d = 1; d <= root; ++d) {
        if (m % d == 0) {
            if (d * d == m)
                ++cnt;
            else
                cnt += 2;  /* d and m/d are distinct */
        }
    }
    return cnt;
}

seq_t a(idx_t n) {
    seq_t total = 0;
    idx_t limit = isqrt_u64(n / 2);          /* floor(sqrt(n/2)) */
    for (idx_t x = 1; x <= limit; ++x) {
        idx_t m = n - x * x;
        total += divisor_count_u64(m);
    }
    return total;
}
