/*
 * A350343  Square numbers k that are abelian orders.
 * https://oeis.org/A350343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 72, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = (m(n))^2
 * where m(n) is the n-th square‑free integer m = p1 · p2 · … · pr
 * such that for all distinct primes pi, pj
 *      pi^2 ≠ 1 (mod pj)   (equivalently (pi^2−1) not divisible by pj).
 * This is OEIS A350342 with the square applied. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* The following helper is needed automatically in the build environment:
 *   seq_t  – unsigned integer type (or arbitrary‑precision)
 *   idx_t  – unsigned 64‑bit integer type (uint64_t)
 */

#define LIMIT 500000   /* upper bound for the n‑th m for n ≤ 10000 */

/* Smallest prime factor table */
static uint32_t *spf = NULL;
static bool sieve_done = false;

/* Build sieve and SPF array up to LIMIT */
static void make_sieve(void)
{
    if (sieve_done) return;
    spf = (uint32_t *)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
    for (uint32_t i = 2; i <= (uint32_t)LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= (uint32_t)LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    sieve_done = true;
}

/* Check whether m is a member of A350342. */
static bool is_valid(unsigned long long m)
{
    if (m == 1) return true;               /* 1 is always allowed */
    unsigned long long tmp = m;
    unsigned long long primes[32];
    size_t pc = 0;

    /* factorisation using SPF */
    while (tmp > 1) {
        unsigned int p = spf[(size_t)tmp];
        if (p == 0) p = (unsigned int)tmp;      /* should not happen for m <= LIMIT */

        tmp /= p;
        if (tmp % p == 0) return false;         /* square factor → not square‑free */

        primes[pc++] = p;
    }

    /* pairwise condition: p_i^2 mod p_j != 1 for all i ≠ j */
    for (size_t i = 0; i < pc; ++i) {
        for (size_t j = i + 1; j < pc; ++j) {
            unsigned long long pi = primes[i];
            unsigned long long pj = primes[j];
            if ((pi * pi) % pj == 1ULL) return false;
            if ((pj * pj) % pi == 1ULL) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    make_sieve();

    size_t found = 0;
    for (unsigned long long m = 1; m <= (unsigned long long)LIMIT; ++m) {
        if (is_valid(m)) {
            ++found;
            if (found == n) {
                seq_t val = (seq_t)m * (seq_t)m;
                return val;
            }
        }
    }

    /* If we reach here the limit was insufficient; recompute with a larger limit
     * (unlikely for n ≤ 10 000).  For robustness we double the limit until
     * the desired term is found.  This fallback uses the same algorithm. */
    unsigned long long limit = LIMIT;
    while (1) {
        limit *= 2;
        if (limit > 10000000ULL) limit = 10000000ULL;   /* safety cap */

        /* Rebuild sieve for the new limit */
        free(spf);
        spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
        for (uint32_t i = 2; i <= (uint32_t)limit; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                if ((uint64_t)i * i <= limit) {
                    for (uint32_t j = i * i; j <= (uint32_t)limit; j += i)
                        if (spf[j] == 0) spf[j] = i;
                }
            }
        }

        found = 0;
        for (unsigned long long m = 1; m <= (unsigned long long)limit; ++m) {
            if (is_valid(m)) {
                ++found;
                if (found == n) {
                    seq_t val = (seq_t)m * (seq_t)m;
                    return val;
                }
            }
        }
    }
}
