/*
 * A389734  Standard composition numbers of compositions with all distinct
 *          0-prepended first differences.
 * https://oeis.org/A389734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th non‑negative integer k such that the
 *            0‑prepended first differences of the composition associated with
 *            k (via the standard bijection with the binary representation)
 *            are all distinct
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool good_k(idx_t k)
{
    /* k = 0 corresponds to the empty composition and always qualifies. */
    if (k == 0ULL)
        return true;

    /* Store the gaps between successive set bits of k.  </gap> */
    idx_t gaps[64];
    idx_t cnt = 0;             /* number of gaps (= number of set bits)   */
    idx_t prev = 0;            /* position of the previous set bit        */
    idx_t pos  = 1;            /* current bit position (1‑based)          */
    idx_t t    = k;

    while (t) {
        if (t & 1ULL) {
            gaps[cnt++] = pos - prev;
            prev = pos;
        }
        t >>= 1;
        pos++;
    }

    /* Compute the 0‑prepended first differences of the composition
       (in reverse order of the gaps).  The resulting values can be
       negative; int64_t is enough for the differences. */
    int64_t d[64];

    d[0] = (int64_t)gaps[cnt - 1];
    for (idx_t i = 1; i < cnt; ++i) {
        d[i] = (int64_t)gaps[cnt - 1 - i] - (int64_t)gaps[cnt - i];
    }

    /* Check that all elements of d[] are distinct.  m ≤ 64 here. */
    for (idx_t i = 0; i < cnt; ++i) {
        for (idx_t j = i + 1; j < cnt; ++j) {
            if (d[i] == d[j])
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* We must return the n‑th qualifying integer k (1‑based indexing). */
    idx_t seen = 0;
    for (idx_t k = 0; ; ++k) {
        if (good_k(k)) {
            ++seen;
            if (seen == n)
                return (seq_t)k;
        }
    }
}
