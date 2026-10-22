/*
 * A399769  Maximum length of a strictly increasing sequence of positive
 *          divisors d_1, ..., d_k of n such that d_i divides d_{i+1} - 1 for
 *          1 <= i < k.
 * https://oeis.org/A399769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A399769: maximum length of a strictly increasing chain of
   positive divisors d1 < d2 < … < dk of n such that di divides (di+1 - 1).
   Algorithm: enumerate all divisors of n, sort them, then use dynamic
   programming to find the longest chain.  The runtime is
   O(τ(n)^2), fast enough for n up to 10 000.  All calculations are done
   with native unsigned 64‑bit integers; the result is cast to seq_t. */

#include <stdlib.h>
#include <stdint.h>

/* Comparer for qsort on uint64_t */
static int cmp_uint64(const void *p1, const void *p2)
{
    const uint64_t *x = (const uint64_t *)p1;
    const uint64_t *y = (const uint64_t *)p2;
    return (*x < *y) ? -1 : ((*x > *y) ? 1 : 0);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* ---- Factorise n ---- */
    uint64_t m = (uint64_t)n;
    uint64_t primes[32];
    size_t exps[32];
    size_t pc = 0;
    size_t est = 1;          /* estimate of number of divisors */

    /* trial division – fast enough for n up to 10 000 */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            size_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pc]   = p;
            exps[pc]     = e;
            est *= (e + 1);
            ++pc;
        }
    }
    if (m > 1) {                /* remaining prime factor */
        primes[pc] = m;
        exps[pc]   = 1;
        est *= 2;
        ++pc;
    }

    /* ---- Generate all divisors ---- */
    uint64_t *divs = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!divs) return (seq_t)0;    /* allocation failure */

    divs[0] = 1;
    size_t dlen = 1;

    for (size_t i = 0; i < pc; ++i) {
        uint64_t p   = primes[i];
        size_t    e  = exps[i];
        size_t cur_len = dlen;

        uint64_t pow = 1;
        for (size_t exp = 1; exp <= e; ++exp) {
            pow *= p;
            for (size_t j = 0; j < cur_len; ++j) {
                divs[dlen++] = divs[j] * pow;
            }
        }
    }

    /* ---- Sort divisors ---- */
    qsort(divs, dlen, sizeof(uint64_t), cmp_uint64);

    /* ---- Dynamic programming for longest chain ---- */
    size_t *dp = (size_t *)calloc(dlen, sizeof(size_t));
    if (!dp) {
        free(divs);
        return (seq_t)0;
    }

    size_t best = 0;
    for (size_t i = 0; i < dlen; ++i) {
        dp[i] = 1;
        for (size_t j = 0; j < i; ++j) {
            if ((divs[i] - 1ULL) % divs[j] == 0ULL) {
                size_t cand = dp[j] + 1;
                if (cand > dp[i]) dp[i] = cand;
            }
        }
        if (dp[i] > best) best = dp[i];
    }

    free(divs);
    free(dp);

    return (seq_t)best;
}
