/*
 * A348379  Number of factorizations of n with an alternating permutation.
 * https://oeis.org/A348379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of weakly increasing factorizations of n > 1 such
 *            that the multiset of factors admits an alternating permutation
 *            (strictly increasing and decreasing alternately)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Recursive search for an alternating permutation of a given multiset of factors. */
static bool backtrack(const seq_t *parts, unsigned m, bool used[], unsigned pos, seq_t prev,
                      bool greaterNext)
{
    if (pos == m)
        return true;                      /* a complete alternating sequence found */

    for (unsigned i = 0; i < m; ++i) {
        if (used[i])
            continue;
        seq_t val = parts[i];
        if (greaterNext) {                /* need val > prev */
            if (val <= prev)
                continue;
        } else {                           /* need val < prev */
            if (val >= prev)
                continue;
        }
        used[i] = true;
        if (backtrack(parts, m, used, pos + 1, val, !greaterNext))
            return true;
        used[i] = false;
    }
    return false;
}

static bool try_pattern(const seq_t *parts, unsigned m, bool startGreater)
{
    bool used[64];
    memset(used, 0, sizeof(used));

    for (unsigned i = 0; i < m; ++i) {
        used[i] = true;
        if (m == 1)
            return true;
        if (backtrack(parts, m, used, 1, parts[i], startGreater))
            return true;
        used[i] = false;
    }
    return false;
}

static bool hasAlternating(const seq_t *parts, unsigned m)
{
    if (m == 0)
        return false;
    if (m == 1)
        return true;
    return try_pattern(parts, m, true) || try_pattern(parts, m, false);
}

/* Enumerate all weakly increasing factorizations of n and count those with an alternating
   permutation of the factors. */
static void enumerate(uint64_t rem, uint64_t min_fac, seq_t *parts,
                      unsigned len, seq_t *total)
{
    if (rem == 1) {
        if (hasAlternating(parts, len))
            (*total)++;
        return;
    }
    for (uint64_t d = min_fac; d <= rem; ++d) {
        if (rem % d == 0) {
            parts[len] = (seq_t)d;
            enumerate(rem / d, d, parts, len + 1, total);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;            /* by definition a(1) = 1 */

    static seq_t parts[64];
    seq_t total = 0;
    enumerate(n, 2, parts, 0, &total);
    return total;
}
