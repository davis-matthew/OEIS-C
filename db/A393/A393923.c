/*
 * A393923  Product of numbers k <= n that have the same squarefree kernel as
 *          n.
 * https://oeis.org/A393923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = product of all k ≤ n such that rad(k) = rad(n)
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 71, u64 n <= 161,
 *            u128 n <= 575, big any n
 * Helpers:   factor_u64, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation factorizes n, then enumerates all
   numbers k ≤ n whose prime factor set equals that of n.
   Each such k is multiplied into the running product. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"

/* Recursive helper: multiply all products formed from primes
   starting at index `idx` that do not exceed `limit`. */
static void _dfs_product(uint64_t limit, uint64_t prod,
                         const uint64_t *primes, size_t idx,
                         size_t pcnt, seq_t *res)
{
    if (idx == pcnt) {
        *res = seq_mul(*res, (seq_t)prod);
        return;
    }

    uint64_t p = primes[idx];
    uint64_t power = 1;

    /* iterate over powers p^e with e ≥ 1 */
    while (true) {
        if (power > limit / p) break;          /* would overflow */
        power *= p;
        if (prod > limit / power) break;       /* product too large */

        uint64_t new_prod = prod * power;
        if (new_prod > limit) break;

        _dfs_product(limit, new_prod, primes, idx + 1, pcnt, res);
    }
}

/* Main function required by the judging framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* safety for offset 1 */

    uint64_t N = (uint64_t)n;

    /* factorization of N */
    uint64_t primes[64];
    uint64_t exps[64];                       /* not used, only to satisfy the API */
    size_t pcnt;

    factor_u64(N, primes, exps, &pcnt);

    seq_t result = 1;
    _dfs_product(N, 1, primes, 0, pcnt, &result);

    return result;
}
