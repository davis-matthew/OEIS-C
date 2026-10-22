/*
 * A360663  a(n) is the least integer m >= 3 such that n is a centered m-gonal
 *          number.
 * https://oeis.org/A360663
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer m >= 3 such that * n = 1 + m * k * (k-1) /
 *            2 for some integer k >= 2
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 4) return 0;                     /* below the offset */
    seq_t n_s = (seq_t)n;                    /* use seq_t for all work */
    seq_t total = (seq_t)2 * (n_s - 1);      /* 2 * (n-1) */

    seq_t best_m = 0;
    for (idx_t k = 2; ; ++k) {
        seq_t k_s = (seq_t)k;
        seq_t denom = k_s * (k_s - 1);       /* k * (k-1) */

        /* If denom > total/3, then m < 3, so stop searching. */
        if (denom > total / 3) break;
        if (total % denom != 0) continue;   /* denom is not a divisor of total */

        seq_t m = total / denom;
        if (best_m == 0 || m < best_m) best_m = m;
    }
    return best_m;
}
