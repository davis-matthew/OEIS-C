/*
 * A385494  Total number of 1's in the decimal digits of the divisors of n.
 * https://oeis.org/A385494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{d|n} (# of decimal digits '1' in d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Recursive enumeration of all divisors from the prime factorisation. */
static void dfs(const uint64_t *pr, const uint64_t *ex,
                size_t pos, size_t cnt,
                uint64_t cur, seq_t *total)
{
    if (pos == cnt) {
        uint64_t d = cur;
        while (d) {
            if ((d % 10) == 1)
                (*total)++;
            d /= 10;
        }
        return;
    }
    uint64_t mult = 1;
    for (size_t e = 0; e <= ex[pos]; ++e) {
        dfs(pr, ex, pos + 1, cnt, cur * mult, total);
        mult *= pr[pos];
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset is 1; undefined for 0 */

    uint64_t nn = (uint64_t)n;
    /* Arrays large enough for all distinct prime factors of a 64‑bit integer. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64(nn, pr, ex, &cnt);

    seq_t total = 0;
    dfs(pr, ex, 0, cnt, 1, &total);
    return total;
}
