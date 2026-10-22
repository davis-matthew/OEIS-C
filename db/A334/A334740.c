/*
 * A334740  Number of unordered factorizations of n with 3 different parts >
 *          1.
 * https://oeis.org/A334740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Counts unordered factorizations of n into parts > 1 with exactly
 *            3 distinct part values
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void _dfs(unsigned long long rem, unsigned long long start,
                unsigned long long *parts, int depth, seq_t *cnt)
{
    for (unsigned long long f = start; f * f <= rem; ++f) {
        if (rem % f == 0) {
            parts[depth] = f;
            _dfs(rem / f, f, parts, depth + 1, cnt);
        }
    }
    /* finish the factorization by adding the remaining part */
    if (rem >= start) {
        parts[depth] = rem;
        /* count distinct values */
        int distinct = 0;
        for (int i = 0; i <= depth; ++i) {
            int j;
            for (j = 0; j < i; ++j)
                if (parts[i] == parts[j]) break;
            if (j == i) ++distinct;
        }
        if (distinct == 3)
            ++(*cnt);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, return 0 */
    if (n < 2) return 0;           /* no factorization of 1 into >1 parts */

    unsigned long long rem = (unsigned long long)n;
    unsigned long long parts[64];
    seq_t count = 0;

    _dfs(rem, 2, parts, 0, &count);
    return count;
}
