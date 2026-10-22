/*
 * A327593  Numbers m where an integer b that is a power of two > 2 with 1 < b
 *          < m exists such that m is a base-b repdigit.
 * https://oeis.org/A327593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A327593: Numbers m such that ∃ a base b=2^k (k≥2), with 1<b<m,
   where the representation of m in base b is a repdigit. */
/* The algorithm sequentially tests integers for the property, caches
   computed values, and returns the n-th term (1‑based index). */

#include <stddef.h>
#include <stdbool.h>

static inline bool is_candidate(seq_t m)
{
    /* For each power of two base b (>=4), try digit lengths t>=2. */
    for (seq_t b = 4; b < m; b <<= 1) {
        seq_t mmuls = m * (b - 1);
        /* Enumerate powers of b: pow = b^t for t=2,3,... */
        seq_t pow = b * b;                 /* b^2 */
        while (pow <= mmuls + 1) {         /* continue while (b^t-1) <= mmuls */
            seq_t denom = pow - 1;          /* b^t - 1 */
            if (mmuls % denom == 0) {
                seq_t d = mmuls / denom;
                if (d > 0 && d < b) {
                    return true;           /* found a repdigit representation */
                }
            }
            /* Next power of b */
            pow = pow * b;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* cache of computed values: cache[0] = a(1), ... */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;          /* number of terms stored */
    static seq_t last_m = 0;        /* last term computed */

    if (n == 0) return 0;           /* undefined, but safe return */

    /* Grow cache to accommodate n terms */
    if (len < (size_t)n) {
        /* Start scanning from the next integer after last_m */
        for (seq_t m = last_m + 1;
             len < (size_t)n;
             ++m) {
            if (is_candidate(m)) {
                /* Ensure capacity */
                if (len + 1 > cap) {
                    /* grow by doubling strategy */
                    size_t new_cap = cap ? cap * 2 : 16;
                    if (new_cap < len + 1) new_cap = len + 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* allocation failed, return 0 */
                        return 0;
                    }
                    cache = tmp;
                    cap = new_cap;
                }
                cache[len++] = m;
                last_m = m;
            }
        }
    }
    return cache[(size_t)n - 1];
}
