/*
 * A363627  a(n) = greatest product < n of some subset of the divisors of n,
 *          or if n is in A008578 then a(n) = n.
 * https://oeis.org/A363627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest product < n of some subset of the divisors of n,
 *            or if n is in A008578 then a(n) = n
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Special cases: n = 1 or prime */
    if (n <= 1) return (seq_t)n;              /* a(1) = 1 */
    if (is_prime((uint64_t)n)) return (seq_t)n;/* primes map to themselves */

    /* n is composite.  Find all proper divisors (excluding n itself). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Generate all divisors.  Use a fixed array big enough. */
    idx_t divs[256];
    size_t dcnt = 1;                 /* start with divisor 1 */
    divs[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t power = 1;
        size_t cur = dcnt;
        for (uint64_t e = 1; e <= exps[i]; ++e) {
            power *= p;
            for (size_t j = 0; j < cur; ++j)
                divs[dcnt++] = divs[j] * power;
        }
    }

    /* DP over product values < n.  Size n is safe for n <= 10000. */
    size_t sz = (size_t)n;
    bool *reach = (bool *)malloc(sz * sizeof(*reach));
    if (!reach) return (seq_t)n;      /* fallback, should not happen */
    memset(reach, 0, sz * sizeof(*reach));
    reach[1] = true;                  /* empty subset product = 1 */

    /* 0/1 knapsack using multiplication. */
    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divs[i];
        if (d <= 1 || d >= n) continue;   /* skip trivial / too large */
        /* iterate products in reverse so each divisor is used at most once */
        for (size_t prod = sz - 1; prod >= 1; --prod) {
            if (reach[prod]) {
                /* avoid overflow: prod * d < n == sz */
                if (prod <= (n - 1) / d) {
                    reach[prod * d] = true;
                }
            }
            if (prod == 1) break;  /* avoid underflow of size_t */
        }
    }

    /* Find the largest reachable product < n. */
    idx_t best = 1;                       /* at least 1 is reachable */
    for (size_t p = sz - 1; p >= 1; --p) {
        if (reach[p]) {
            best = p;
            break;
        }
        if (p == 1) break;
    }

    free(reach);
    return (seq_t)best;
}
