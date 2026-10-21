/*
 * A256909  Enhanced triangular-number representations, concatenated.
 * https://oeis.org/A256909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth element of the concatenation of the greedy
 *            representations * of all non‑negative integers m using the
 *            enhanced basis B = {0,1,2,3,6,10,15,21,...}. * For each m, * let
 *            g(m) = max{b∈B | b ≤ m} (the greatest element of B ≤ m), * and
 *            the greedy representation of m is obtained by repea...
 * Range:     u8 n <= 693, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t g_of(idx_t x)
{
    if (x < 3)          /* 0,1,2 are in B themselves */
        return (seq_t)x;

    idx_t low  = 0;
    idx_t high = x;      /* k(k+1)/2 ≤ x, k ≤ x */
    idx_t best = 0;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= x) {
            best = mid;
            low  = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    seq_t tri = (seq_t)((unsigned __int128)best * (best + 1) / 2);
    return tri;
}

static inline idx_t count_rep_terms(idx_t x)
{
    if (x == (idx_t)0)      /* representation of 0 is the single term 0 */
        return 1;
    idx_t len = 0;
    while (x > (idx_t)0) {
        idx_t g = (idx_t)g_of(x);
        x -= g;
        ++len;
    }
    return len;
}

/* External linkage function required by the build. */
seq_t a(idx_t n)
{
    idx_t cumulative = 0;
    for (idx_t m = 0; ; ++m) {
        idx_t len = count_rep_terms(m);
        if (cumulative + len > n) {                 /* m contains the desired term */
            idx_t offset = n - cumulative;          /* 0‑based offset in this representation */
            if (m == (idx_t)0)                      /* special: representation of 0 */
                return (seq_t)0;

            idx_t t = m;
            seq_t term = 0;
            for (idx_t i = 0; i <= offset; ++i) {
                seq_t g = g_of(t);
                term = g;
                t -= (idx_t)g;
            }
            return term;
        }
        cumulative += len;
    }
}
