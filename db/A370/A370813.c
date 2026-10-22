/*
 * A370813  Number of non-condensed integer factorizations of n into unordered
 *          factors > 1.
 * https://oeis.org/A370813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of non‑condensed unordered factorizations of a positive integer n.
   A factorization is non‑condensed iff it is impossible to assign to each factor a
   divisor (possibly 1) such that all assigned divisors are distinct.
   The function enumerates all unordered factorizations of n and counts those
   that are not condensed.  The algorithm is efficient for n up to ~10000. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* -------------- helper for divisor assignment --------------- */
static unsigned char *used = NULL;       /* bitmap of divisors used in a leaf   */
static size_t used_limit = 0;           /* current size of used array          */

/* recursive backtracking: try to assign distinct divisors to factors[idx..k-1] */
static bool _has_assignment(size_t idx, size_t k, const uint64_t *factors)
{
    if (idx == k)
        return true;            /* all factors assigned */

    const uint64_t f = factors[idx];
    for (uint64_t d = 1; d * d <= f; ++d) {
        if (f % d == 0) {
            uint64_t d1 = d;
            uint64_t d2 = f / d;

            if (!used[d1]) {
                used[d1] = 1;
                if (_has_assignment(idx + 1, k, factors)) {
                    used[d1] = 0;
                    return true;
                }
                used[d1] = 0;
            }
            if (d2 != d1 && !used[d2]) {
                used[d2] = 1;
                if (_has_assignment(idx + 1, k, factors)) {
                    used[d2] = 0;
                    return true;
                }
                used[d2] = 0;
            }
        }
    }
    return false;               /* no assignment for this leaf */
}

/* -------------- main enumeration of factorizations --------------- */
static void _dfs_fact(uint64_t rem, uint64_t min_f, const uint64_t *cur,
                      size_t depth, size_t *count, uint64_t N)
{
    if (rem == 1) {
        /* evaluate whether the current (unordered) factor list is non‑condensed */
        size_t k = depth;
        if (k == 0) return;

        /* reset used bitmap */
        memset(used, 0, (N + 1) * sizeof(unsigned char));
        if (!_has_assignment(0, k, cur))
            (*count)++;
        return;
    }

    for (uint64_t f = min_f; f <= rem; ++f) {
        if (rem % f == 0) {
            uint64_t new_cur[64];
            memcpy(new_cur, cur, depth * sizeof(uint64_t));
            new_cur[depth] = f;
            _dfs_fact(rem / f, f, new_cur, depth + 1, count, N);
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    const uint64_t N = (uint64_t)n;

    /* allocate bitmap of divisors if necessary */
    if (used == NULL || used_limit <= N) {
        free(used);
        used_limit = N + 1;
        used = (unsigned char *)malloc(used_limit * sizeof(unsigned char));
    }

    size_t cnt = 0;
    _dfs_fact(N, 2, NULL, 0, &cnt, N);
    return (seq_t)cnt;
}
