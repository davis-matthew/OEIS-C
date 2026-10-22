/*
 * A352667  Maximum number of induced copies of the paw graph in an n-node
 *          graph.
 * https://oeis.org/A352667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   * a(n) = max over non‑negative integers a,b,c,d with a+b+c+d = n
 *            of * (a+b)*c*d*(c+d-2)/2 + (c+d)*a*b*(a+b-2)/2 * This maximum is
 *            attained when the two pairs (a,b) and (c,d) are as close * to
 *            equal as possible. The simple O(n) algorithm below computes the
 *            * maximum by iterating over the possible...
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t best = 0;
    for (idx_t t = 0; t <= n; ++t) {
        idx_t s = n - t;

        /* split t into a and b as evenly as possible */
        idx_t a = t / 2;
        idx_t b = t - a;
        seq_t ab = (seq_t)a * (seq_t)b;

        /* split s into c and d as evenly as possible */
        idx_t c = s / 2;
        idx_t d = s - c;
        seq_t cd = (seq_t)c * (seq_t)d;

        /* term1 = t * cd * (s-2) / 2  (zero automatically when s<2) */
        seq_t term1 = (seq_t)t * cd *
                      (s > 2 ? (seq_t)(s - 2) : (seq_t)0) / 2;

        /* term2 = s * ab * (t-2) / 2  (zero automatically when t<2) */
        seq_t term2 = (seq_t)s * ab *
                      (t > 2 ? (seq_t)(t - 2) : (seq_t)0) / 2;

        seq_t val = term1 + term2;
        if (val > best)
            best = val;
    }
    return best;
}
