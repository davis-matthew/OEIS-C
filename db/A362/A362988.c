/*
 * A362988  a(n) = lcm({i, i = 1..n}) / Product_{2 <= p < n, p prime} p.
 * https://oeis.org/A362988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1..n) / ∏{p prime | 2 ≤ p < n} p Compute as: a(n) =
 *            ∏{p prime ≤ n} p^( floor(log_p n) – [p < n] ) (n ≥ 0)
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static cache for primes up to the largest n requested. */
static uint64_t *primes_cache   = NULL;   /* array of primes */
static size_t    primes_count   = 0;      /* number of primes in cache */
static idx_t     primes_limit   = 0;      /* biggest n for which cache is valid */

/* Rebuilds the prime cache for all primes ≤ limit using a simple sieve. */
static void recompute_primes_upto(idx_t limit)
{
    if (limit <= primes_limit) return;   /* already enough */

    /* free old cache */
    if (primes_cache) free(primes_cache);

    /* allocate sieve array */
    size_t sieve_sz = (size_t)(limit + 1);
    bool *is_composite = (bool *)calloc(sieve_sz, sizeof(bool));
    if (!is_composite) return;           /* allocation failed, ignore */

    /* a rough upper bound for number of primes ≤ limit is limit/2 */
    size_t est_cnt = sieve_sz / 2;
    primes_cache = (uint64_t *)malloc(est_cnt * sizeof(uint64_t));
    if (!primes_cache) { free(is_composite); return; }

    size_t cnt = 0;
    if (limit >= 2) {
        primes_cache[cnt++] = 2;
    }
    for (idx_t p = 3; p <= limit; p += 2) {
        if (!is_composite[p]) {
            primes_cache[cnt++] = p;
            if ((idx_t)(p * p) <= limit) {
                for (idx_t j = p * p; j <= limit; j += p * 2) {
                    is_composite[j] = true;
                }
            }
        }
    }
    primes_count = cnt;
    primes_limit = limit;

    free(is_composite);
}

/* Computes a(n) as defined above. */
seq_t a(idx_t n)
{
    /* a(0) = a(1) = 1 by definition. */
    if (n < 2) return (seq_t)1;

    /* ensure primes up to n are available */
    recompute_primes_upto(n);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes_cache[i];
        if (p > n) break;

        /* floor(log_p n) via repeated division */
        idx_t exp = 0;
        idx_t tmp = n;
        while (tmp >= (idx_t)p) {
            tmp /= (idx_t)p;
            exp += 1;
        }

        /* subtract one if p < n (denominator contains each such prime once) */
        if (p < n) {
            if (exp == 0) continue;  /* should not happen */
            exp -= 1;
        }

        if (exp == 0) continue;

        /* compute p^exp and multiply into result */
        seq_t pow = 1;
        seq_t base = (seq_t)p;
        idx_t e = exp;
        while (e) {
            if (e & 1) pow *= base;
            base *= base;
            e >>= 1;
        }
        result *= pow;
    }

    return result;
}
