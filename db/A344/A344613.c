/*
 * A344613  Number of rooted binary trees (in which all inner nodes have
 *          precisely two children) with n leaves and with maximal number of
 *          cherries (i.e., maximal number of pendant subtrees with two
 *          leaves).
 * https://oeis.org/A344613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1. For n ≥ 3: • if n is odd: a(n) =
 *            Σ_{i=1}^{(n‑1)/2} a(i) * a(n‑i) • if n is even, let m = n/2:
 *            a(n) = Σ_{x=1}^{⌊m/2⌋, x<m‑x} a(2x) * a(2(m‑x)) + (m even ?
 *            a(m)*(a(m)+1)/2 : 0)
 * Range:     u8 n <= 18, u16 n <= 30, u32 n <= 54, u64 n <= 104,
 *            u128 n <= 202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* cache capacity (index + 1) */

    /* ensure capacity up to n */
    if ((size_t)n >= cap) {
        size_t new_cap = n + 1;
        /* guarantee indices 1 and 2 are available */
        if (n < 2) new_cap = 3;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* initialise base values if this is the first time */
        if (cap == 0) {
            cache[0] = 0;          /* unused */
            cache[1] = 1;
            cache[2] = 1;
        }

        /* compute values from previously filled size up to n */
        size_t start = (cap > 2 ? cap : 3);
        for (size_t i = start; i <= n; ++i) {
            if ((i & 1) == 1) {               /* odd i */
                idx_t m = (i - 1) / 2;
                seq_t sum = 0;
                for (idx_t j = 1; j <= m; ++j) {
                    sum += cache[j] * cache[i - j];
                }
                cache[i] = sum;
            } else {                          /* even i */
                idx_t m = i / 2;
                seq_t sum = 0;
                idx_t limit = m / 2;           /* floor(m/2) */
                for (idx_t x = 1; x <= limit; ++x) {
                    if (x < (m - x)) {
                        sum += cache[2 * x] * cache[2 * (m - x)];
                    } else { /* x == m - x, occurs only when m even */
                        seq_t k = cache[2 * x];
                        sum += (k * k + k) / 2;
                    }
                }
                cache[i] = sum;
            }
        }

        cap = new_cap;
    }

    return cache[n];
}
