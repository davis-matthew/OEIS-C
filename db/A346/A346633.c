/*
 * A346633  Sum of even-indexed parts (even bisection) of the n-th composition
 *          in standard order.
 * https://oeis.org/A346633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = sum of parts at even indices (2,4,…) of the n‑th
 *            composition in standard order (graded reverse‑lexicographic)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t popcount_idx(idx_t x) {
    idx_t c = 0;
    while (x) {
        c++;
        x &= x - 1;
    }
    return c;
}

seq_t a(idx_t n) {
    /* n == 0 yields the empty composition → sum 0 */
    if (n == 0)
        return 0;

    /* Number of parts = popcount(n) */
    idx_t m = popcount_idx(n);
    /* m is always >= 1 because n > 0 */
    seq_t diff[64] = {0};

    idx_t prev_pos = 0;   /* previous set‑bit position (1‑based) */
    idx_t bit_index = 0;  /* current bit index (0‑based) */
    idx_t idx = 0;        /* index in diff array */

    /* Scan bits of n from LSB to MSB */
    idx_t t = n;
    while (t) {
        if (t & 1) {
            idx_t pos = bit_index + 1;           /* 1‑based position */
            diff[idx++] = (seq_t)(pos - prev_pos); /* difference to previous */
            prev_pos = pos;
        }
        bit_index++;
        t >>= 1;
    }

    /* Sum even‑indexed parts (1‑based indices 2,4,…).
       Those correspond to diff indices m-2, m-4, …          */
    seq_t sum = 0;
    if (m >= 2) {
        int i = (int)m - 2;                      /* start with first eligible index */
        while (i >= 0) {
            sum += diff[i];
            i -= 2;
        }
    }

    return sum;
}
