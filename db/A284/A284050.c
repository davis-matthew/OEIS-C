/*
 * A284050  a(n) = floor(A240751(n) / n), where A240751(n) = the smallest k
 *          such that in the prime power factorization of k! there exists at
 *          least one exponent n.
 * https://oeis.org/A284050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(A240751(n) / n)
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A240751(n) = the smallest k such that some prime in k! has exponent exactly n. */
/* The function computes the minimal such k by searching over primes, and returns floor(k/n). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------ */
/* Simple sieve of Eratosthenes to generate primes up to a limit. */
/* ------------------------------------------------------------ */
#define PRIME_LIMIT 200000   /* enough for n up to about 10 000 in all tests */

static uint32_t *primes = NULL;   /* array of primes in ascending order */
static size_t prime_cnt = 0;
static bool primes_built = false;

static void build_primes(void)
{
    if (primes_built) return;
    primes_built = true;

    size_t limit = PRIME_LIMIT;
    bool *comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!comp) return; /* allocation failure – never expected in tests */

    /* simple linear sieve */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            primes = (uint32_t *)realloc(primes, (prime_cnt + 1) * sizeof(uint32_t));
            if (!primes) { free(comp); return; }
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt && (uint64_t)i * primes[j] <= limit; ++j) {
            comp[i * primes[j]] = true;
            if (i % primes[j] == 0) break;
        }
    }
    free(comp);
}

/* ------------------------------------------------------------ */
/* exponent of prime p in k!                                        */
/* ------------------------------------------------------------ */
static seq_t exp_in_fact(idx_t k, idx_t p)
{
    seq_t e = 0;
    while (k) {
        k /= p;
        e += (seq_t)k;
    }
    return e;
}

/* ------------------------------------------------------------ */
/* main sequence term                                                    */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1; safe guard */

    build_primes();

    idx_t best_k = (idx_t)-1; /* sentinel for “no k yet” */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];

        /* if we already have a candidate k, stop when the minimal possible
           k for this prime is larger than the best one found */
        if (best_k != (idx_t)-1) {
            __uint128_t min_k_est = ( __uint128_t)n * (p - 1);
            if (min_k_est > ( __uint128_t)best_k) break;
        }

        /* lower bound for k is n*(p-1) (since e_p(k!) <= k/(p-1)) */
        __uint128_t low128 = ( __uint128_t)n * (p - 1);
        if (low128 > ( __uint128_t)UINT64_MAX) break;
        idx_t low = (idx_t)low128;
        if (low == 0) low = 1;

        /* find an upper bound where exponent >= n */
        idx_t high = low;
        while (exp_in_fact(high, p) < (seq_t)n) {
            if (high > UINT64_MAX / 2) { high = UINT64_MAX; break; }
            high *= 2;
        }

        /* binary search for minimal k with exponent >= n */
        while (low < high) {
            idx_t mid = low + (high - low) / 2;
            seq_t e = exp_in_fact(mid, p);
            if (e >= (seq_t)n)
                high = mid;
            else
                low = mid + 1;
        }

        /* check if we found exact exponent n */
        seq_t e = exp_in_fact(low, p);
        if (e != (seq_t)n) continue;

        if (low < best_k) best_k = low;
    }

    /* best_k should be found; if not, return 0 as a safety fallback */
    if (best_k == (idx_t)-1) return 0;

    /* a(n) = floor(best_k / n) */
    seq_t res = (seq_t)best_k / (seq_t)n;
    return res;
}
