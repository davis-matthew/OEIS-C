/*
 * A390551  a(n) = Sum_{i = 0..n} i*A070939(i).
 * https://oeis.org/A390551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i = 1..n} i * (floor(log2(i)) + 1)
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0 */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    seq_t total = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t pow2 = (idx_t)1 << (k - 1);   /* 2^(k-1) */
        if (pow2 > n)
            break;

        idx_t nextPow2 = pow2 << 1;        /* 2^k */
        idx_t end = (nextPow2 - 1 <= n) ? (nextPow2 - 1) : n;

        idx_t cnt = end - pow2 + 1;

        seq_t sum_i = ((seq_t)pow2 + (seq_t)end) * (seq_t)cnt / 2;
        total += (seq_t)k * sum_i;
    }
    return total;
}
