/*
 * A349176  Odd numbers k for which gcd(k, A003961(k)) = gcd(sigma(k),
 *          A003961(k)) > 1, where A003961(n) is fully multiplicative with
 *          a(prime(k)) = prime(k+1), and sigma is the sum of divisors
 *          function.
 * https://oeis.org/A349176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth odd integer k such that gcd(k, A003961(k)) =
 *            gcd(σ(k), A003961(k)) > 1, where A003961 is fully multiplicative
 *            with a(p) = next prime after p
 * Range:     u8 none, u16 n <= 38, u32 n <= 8050, u64 n <= 8050,
 *            u128 n <= 8050, big any n
 * Helpers:   factor_u64, gcd, grow_seq, next_prime, seq_mul, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"               /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/sigma_of.h"          /* seq_t sigma_of(idx_t n); */
#include "../utils/factor_u64.h"        /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/next_prime.h"        /* uint64_t next_prime(uint64_t start); */
#include "../utils/seq_mul.h"           /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/grow_seq.h"          /* void grow_seq(seq_t **arr, size_t *cap, size_t need); */

static seq_t *cache = NULL;     /* cached values a(1), a(2), ... */
static size_t cache_len = 0;   /* number of cached values */
static size_t cache_cap = 0;   /* capacity of cache array */
static uint64_t last_k = 1;    /* last tested odd integer */

/* Compute A003961(k) for a 64‑bit integer k.
   A003961 is multiplicative with a(p) = next_prime(p). */
static seq_t compute_A003961(uint64_t k)
{
    if (k == 1) return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t nextp = next_prime(p);
        seq_t base = (seq_t)nextp;
        seq_t pow = (seq_t)1;
        uint64_t exp = exps[i];

        /* fast exponentiation using seq_mul */
        while (exp > 0) {
            if (exp & 1) pow = seq_mul(pow, base);
            base = seq_mul(base, base);
            exp >>= 1;
        }
        result = seq_mul(result, pow);
    }
    return result;
}

/* Return the nth element of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but defensive */

    if ((size_t)n <= cache_len) {
        return cache[(size_t)n - 1];
    }

    /* Continue filling the cache until we have n values. */
    while (cache_len < (size_t)n) {
        /* Find next odd integer. */
        do {
            ++last_k;
        } while ((last_k & 1) == 0);

        seq_t ak = compute_A003961(last_k);
        seq_t kseq = (seq_t)last_k;

        seq_t g1 = gcd(kseq, ak);
        if (g1 <= 1) continue;        /* require > 1 */

        seq_t sigma = sigma_of(last_k);
        seq_t g2 = gcd(sigma, ak);
        if (g2 != g1) continue;

        /* Store the valid k. */
        grow_seq(&cache, &cache_cap, cache_len + 1);
        cache[cache_len++] = kseq;
    }

    return cache[(size_t)n - 1];
}
