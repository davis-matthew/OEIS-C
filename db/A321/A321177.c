/*
 * A321177  Heinz numbers of integer partitions that are the vertex-degrees of
 *          some set system with no singletons.
 * https://oeis.org/A321177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = nth integer such that in its prime factorisation n = Π
 *            p_k^{e_k} (where p_k is the k‑th prime) let S = Σ e_k (total
 *            number of prime factors, counted with multiplicity) and L =
 *            max{k | e_k > 0} (index of the largest prime used). The integer
 *            belongs to the sequence iff L ≤ S – 1
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ----------  Prime precomputation  ---------- */
#define SIEVE_LIMIT 2000000
static uint32_t primeIdxArray[SIEVE_LIMIT + 1];
static uint32_t primesList[250000];
static bool composite[SIEVE_LIMIT + 1];
static size_t primeCount = 0;

static void build_primes(void)
{
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!composite[i]) {
            primeIdxArray[i] = ++primeCount;
            if (primeCount <= 250000)
                primesList[primeCount - 1] = (uint32_t)i;
            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    composite[j] = true;
            }
        }
    }
}

/* ----------  Cache for sequence values  ---------- */
#define CACHE_MAX 200000
static seq_t cache[CACHE_MAX];
static size_t cacheSize = 0;
static idx_t nextCandidate = 2;
static bool initialized = false;

/* ----------  Main sequence function  ---------- */
seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (!initialized) {
        cacheSize = 0;
        cache[cacheSize++] = (seq_t)1;    /* a(1) = 1 */
        build_primes();
        nextCandidate = 2;
        initialized = true;
    }

    while (cacheSize < (size_t)n) {
        idx_t cand = nextCandidate++;
        idx_t tmp = cand;
        idx_t S = 0;           /* total exponent sum */
        idx_t L = 0;           /* maximum prime index */

        /* Factorisation using precomputed primes */
        for (size_t i = 0; i < primeCount &&
                         (uint64_t)primesList[i] * primesList[i] <= tmp;
             ++i) {
            uint32_t p = primesList[i];
            if (tmp % p == 0) {
                uint32_t e = 0;
                do {
                    tmp /= p;
                    ++e;
                } while (tmp % p == 0);
                S += e;
                uint32_t idx = (uint32_t)(i + 1);   /* 1‑based index of prime */
                if (idx > L) L = idx;
            }
        }

        if (tmp > 1) {                     /* remaining prime factor */
            ++S;
            uint32_t idx = primeIdxArray[(uint32_t)tmp];
            if (idx > L) L = idx;
        }

        /* Check the condition L ≤ S – 1 (cand > 1 ensures S > 0) */
        if (cand > 1 && S > 0 && L <= (S - 1)) {
            if (cacheSize < CACHE_MAX)
                cache[cacheSize++] = (seq_t)cand;
        }
    }

    return cache[(size_t)n - 1];
}
