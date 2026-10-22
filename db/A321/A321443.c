/*
 * A321443  Number of "bilaterally symmetric hexagonal partitions" of n.
 * https://oeis.org/A321443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (k, m, r) with 1 ≤ k ≤ m, r ≥ 1 * such
 *            that (k + m - 1) * (m - k) + r * m = n; a(0) = 1
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumes seq_t and idx_t are defined elsewhere (by the build). */
seq_t a(idx_t n)
{
    /* Special case: the empty partition. */
    if (n == 0)
        return 1;

    seq_t count = 0;

    /* Enumerate all possible starting parts k. */
    for (idx_t k = 1; k <= n; ++k) {
        /* Enumerate all possible largest parts m ≥ k. */
        for (idx_t m = k; ; ++m) {
            seq_t kk = (seq_t)k;
            seq_t mm = (seq_t)m;

            /* B = (k + m - 1) * (m - k)  (sum of twice the smaller parts). */
            seq_t B = (kk + mm - 1) * (mm - kk);

            /* If m > k and B already exceeds n, no further m will work. */
            if (m > k && B >= (seq_t)n)
                break;

            seq_t t = (seq_t)n - B;  /* Remaining amount to be filled by r * m. */

            if (t <= 0)   /* No valid r (needs r ≥ 1). */
                continue;

            /* r = t / m must be an integer. */
            if (t % mm == 0)
                count += 1;
        }
    }
    return count;
}
