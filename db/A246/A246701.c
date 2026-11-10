/*
 * A246701  First differences of A246520.
 * https://oeis.org/A246701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A246520(n+1) - A246520(n), where A246520(n) =
 *            max_{0<=k<=n} (binary concatenation of (n-k) and (n+k));
 *            computed by direct search over k
 * Range:     u8 n <= 10, u16 n <= 387, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 955 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A246520_a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t best = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t a_part = (seq_t)n - (seq_t)k;
        seq_t b_part = (seq_t)n + (seq_t)k;

        idx_t t = 0;
        seq_t tmp = b_part;
        while (tmp) { tmp >>= 1; ++t; }

        seq_t val = (a_part << t) | b_part;

        if (val > best)
            best = val;
    }

    return best;
}

seq_t a(idx_t n)
{
    return A246520_a(n + 1) - A246520_a(n);
}
