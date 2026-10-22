/*
 * A327500  Number of steps to reach a fixed point starting with n and
 *          repeatedly taking the quotient by the maximum divisor whose prime
 *          multiplicities are distinct (A327498, A327499).
 * https://oeis.org/A327500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps needed to reach 1 by repeatedly dividing
 *            n by the largest divisor whose prime multiplicities are distinct
 * Range:     u8 n <= 9953, u16 n <= 9953, u32 n <= 9953, u64 n <= 9953,
 *            u128 n <= 9953, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Recursive helper to enumerate all divisors and keep the largest
   one with distinct exponents.  ex array holds the prime exponents
   of n. */
static void _rec_divisor(size_t idx, uint64_t cur, uint64_t *pr, uint64_t *ex,
                         size_t cnt, int *sel, uint64_t *best)
{
    if (idx == cnt) {
        /* Check if the selected exponents sel[] are all distinct. */
        bool ok = true;
        bool seen[32] = { false };  /* exponents are tiny for n <= 10000 */
        for (size_t j = 0; j < cnt; ++j) {
            uint64_t e = sel[j];
            if (e == 0) continue;
            if (seen[e]) { ok = false; break; }
            seen[e] = true;
        }
        if (ok && cur > *best) *best = cur;
        return;
    }
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        sel[idx] = (int)e;
        uint64_t newcur = cur;
        for (uint64_t k = 0; k < e; ++k) newcur *= pr[idx];
        _rec_divisor(idx + 1, newcur, pr, ex, cnt, sel, best);
    }
}

/* Return the maximum divisor of n whose prime multiplicities are distinct. */
static uint64_t _max_distinct_div(uint64_t n)
{
    if (n == 1) return 1;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);

    uint64_t best = 1;
    int sel[64] = {0};
    _rec_divisor(0, 1, pr, ex, cnt, sel, &best);
    return best;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return 0;
    seq_t steps = 0;
    uint64_t cur = (uint64_t)n;

    while (cur > 1) {
        uint64_t d = _max_distinct_div(cur);
        cur /= d;
        ++steps;
    }
    return steps;
}
