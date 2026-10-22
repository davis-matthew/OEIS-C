/*
 * A363035  Sum of divisors of 5*n-3 of form 5*k+3.
 * https://oeis.org/A363035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of divisors of (5*n-3) that are congruent to 3 modulo
 *            5
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static void dfs(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                size_t pos, uint64_t cur, seq_t *sum)
{
    if (pos == cnt) {
        if ((cur % 5ULL) == 3ULL) {
            *sum += (seq_t)cur;
        }
        return;
    }
    uint64_t p = pr[pos];
    uint64_t e = ex[pos];
    uint64_t mult = 1ULL;
    for (uint64_t k = 0; k <= e; ++k) {
        dfs(pr, ex, cnt, pos + 1, cur * mult, sum);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n * 5ULL - 3ULL;      /* 5*n-3 */
    if (m <= 1) return 0;                        /* no divisors ≡3 (mod 5) */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);           /* factor m */

    seq_t result = 0;
    dfs(primes, exps, cnt, 0, 1ULL, &result);    /* enumerate divisors */

    return result;
}
