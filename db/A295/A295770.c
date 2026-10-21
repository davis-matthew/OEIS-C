/*
 * A295770  Numbers k such that Bernoulli number B_{k} has denominator 4686.
 * https://oeis.org/A295770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 70 * m where m is the n-th positive integer * such that
 *            for every prime p ≠ 2,3,11,71, (p-1) does not divide 70*m. * The
 *            function sieves out all integers m that would introduce an *
 *            undesired prime factor in the denominator of Bernoulli B_{70*m}
 * Range:     u8 n <= 1, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _gcd_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/_gcd_idx.h"

/* Euclidean gcd for idx_t (uint64_t) */

seq_t a(idx_t n)
{
    /* ----- static data ----------------------------------------------------- */
    static bool initialized = false;
    static bool *bad = NULL;              /* bad[m] == true if m is forbidden  */
    static size_t bad_cap = 0;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;
    static idx_t current_m = 1;           /* next candidate m to test      */

    /* Choose parameters large enough for n up to ~10000 */
    const idx_t M_MAX = 300000;                    /* maximum m considered          */
    const idx_t PRIME_LIMIT = 70 * M_MAX + 70;      /* up to this prime, we need d=p-1 */

/* ---- initialization ----------------------------------------------------- */
    if (!initialized) {
        /* allocate bad flags */
        bad_cap = (size_t)M_MAX + 1;
        bad = (bool *)calloc(bad_cap, sizeof(bool));
        if (!bad) return (seq_t)0;   /* out of memory fallback */

        /* build primes up to PRIME_LIMIT */
        build_primes_upto(PRIME_LIMIT, &primes, &primes_cnt);
        if (!primes) return (seq_t)0;

        /* mark forbidden multiples of m0 = (p-1)/gcd(p-1,70) for all primes
           except 2, 3, 11, 71 itself */
        const idx_t MULT = 70;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p == 2ULL || p == 3ULL || p == 11ULL || p == 71ULL)
                continue;   /* these primes are always present */
            idx_t d = (idx_t)(p - 1ULL);
            idx_t g = _gcd_idx(MULT, d);
            idx_t m0 = d / g;
            if (m0 == 0 || m0 > M_MAX) continue;
            for (idx_t j = m0; j <= M_MAX; j += m0)
                bad[(size_t)j] = true;
        }

        /* allocate terms array with a modest initial capacity */
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) return (seq_t)0;
        terms_len = 0;
        initialized = true;
    }

/* ---- generate terms up to required index -------------------------------- */
    while (terms_len < (size_t)n) {
        /* find next admissible m */
        for (idx_t m = current_m; m <= M_MAX; ++m) {
            if (bad[(size_t)m]) {
                current_m = m + 1;
                continue;
            }

            /* valid m found */
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;  /* out of memory */
                terms = tmp;
            }
            terms[terms_len++] = (seq_t)(70ULL * (seq_t)m);
            current_m = m + 1;
            break;
        }

        /* If M_MAX exceeded, we would need to extend the sieve.
           For n up to about 10000 with current M_MAX the loop terminates
           long before reaching that limit. */
    }

    return terms[(size_t)(n - 1)];
}
