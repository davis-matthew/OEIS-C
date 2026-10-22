/*
 * A349732  Smallest k such that A349194(k) = n, or 0 if no such k exists.
 * https://oeis.org/A349732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n if n ≤ 9 * min(10*d + u) over d∈[1,9], u∈[0,9] such
 *            that d*(d+u) = n, * otherwise 0. This implements “smallest k
 *            with f(k)=n” * where f(k)=k for single‑digit k, and f(k)=d*(d+u)
 *            for k≥10
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle single‑digit outputs directly. */
    if (n <= 9)
        return (seq_t)n;

    seq_t best = 0;  /* 0 means “not yet found” */

    for (idx_t d = 1; d <= 9; ++d)
    {
        for (idx_t u = 0; u <= 9; ++u)
        {
            seq_t val = (seq_t)d * ((seq_t)d + (seq_t)u);
            if (val == n)
            {
                seq_t cand = (seq_t)10 * (seq_t)d + (seq_t)u;
                if (best == 0 || cand < best)
                    best = cand;
            }
        }
    }

    return best;  /* 0 if no candidate was found */
}
