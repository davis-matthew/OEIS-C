/*
 * A256673  Odd numbers with prime arithmetic derivative A003415.
 * https://oeis.org/A256673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd number whose arithmetic derivative is prime
 * Range:     u8 n <= 4, u16 n <= 1343, u32 n <= 3347, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* available utilities */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */

#define CACHE_MAX 20000   /* safely covers n up to ~10000 */

static seq_t  cache[CACHE_MAX];   /* memoised sequence values   */
static idx_t  cache_len = 0;      /* number of cached items */

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour below the offset is irrelevant */
        return 0;

    /* generate terms until cache contains at least n values */
    while (cache_len < n) {
        /* start from 3 if cache empty, otherwise from next odd after last cached */
        uint64_t cand = (cache_len == 0) ? 3ULL : (uint64_t)cache[cache_len - 1] + 2ULL;

        /* search for the next odd with prime arithmetic derivative */
        while (true) {
            /* factorise cand */
            uint64_t primes[64];
            uint64_t exps[64];
            size_t   cnt;
            factor_u64(cand, primes, exps, &cnt);

            /* need at least 3 distinct prime factors */
            if (cnt < 3) {
                cand += 2ULL;
                continue;
            }

            /* compute arithmetic derivative D = Σ (n/p_i) * e_i */
            seq_t D = 0;
            seq_t n_seq = (seq_t)cand;
            for (size_t i = 0; i < cnt; ++i) {
                seq_t term = n_seq / (seq_t)primes[i];
                term *= (seq_t)exps[i];
                D += term;
            }

            /* check if D is prime */
            if (is_prime_big(D)) {
                if (cache_len < CACHE_MAX)
                    cache[cache_len++] = n_seq;
                else
                    /* should not happen for n <= 10000 */
                    ;
                break;   /* found the next term */
            }

            cand += 2ULL;   /* test next odd candidate */
        }
    }
    return cache[n - 1];
}
