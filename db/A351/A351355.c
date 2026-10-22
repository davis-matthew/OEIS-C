/*
 * A351355  Number of ways the numbers from 1..n do not divide numbers from
 *          n+1..2n.
 * https://oeis.org/A351355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1) + A006218(n) - A006218(2n) * where A006218(n) =
 *            sum_{k=1}^n floor(n/k)
 * Range:     u8 n <= 17, u16 n <= 259, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sum_floor(idx_t N) {
    seq_t total = 0;
    for (idx_t k = 1; k <= N; ++k) {
        total += (seq_t)(N / k);
    }
    return total;
}

seq_t a(idx_t n) {
    /* Compute A006218(n) and A006218(2n) */
    seq_t half = sum_floor(n);
    seq_t twice = sum_floor(n + n);
    /* Compute n*(n+1) as seq_t to avoid overflow of idx_t */
    seq_t n_seq = (seq_t)n;
    seq_t detail = n_seq * (n_seq + 1);
    return detail + half - twice;
}
