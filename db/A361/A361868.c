/*
 * A361868  Positive integers > 1 whose prime indices satisfy (maximum) >=
 *          2*(median).
 * https://oeis.org/A361868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): OEIS A361868 – smallest integers >1 whose multiset of prime indices
 * has maximum ≥ 2·median.  This implementation searches successive integers
 * and tests the prime‑index property by factoring each candidate with a
 * dynamically extended sieve of primes. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;
static size_t primes_len = 0;

/* Ensure that `primes` contains all primes up to at least `n`. */
static void ensure_primes_up_to(uint64_t n)
{
    if (primes != NULL && n <= primes[primes_len - 1])
        return;
    free(primes);
    /* use a limit large enough for the expected range; will grow if needed */
    uint64_t limit = (n > 5000000ULL) ? n : 5000000ULL;
    build_primes_upto(limit, &primes, &primes_len);
}

/* Binary search for the 1‑based index of prime `p`.  `p` is assumed to be prime
 * and present in the `primes` array. */
static idx_t prime_index(uint64_t p)
{
    idx_t lo = 1, hi = (idx_t)primes_len;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        uint64_t val = primes[mid - 1];
        if (val == p)
            return mid;
        if (val < p)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return 0; /* should never happen */
}

/* Test whether a given candidate satisfies the condition. */
static bool satisfies(uint64_t candidate)
{
    if (candidate <= 1)
        return false;

    uint64_t n = candidate;
    /* temporary array for indices; for 64-bit numbers at most 64 factors */
    idx_t idxs[64];
    size_t cnt = 0;
    idx_t mx = 0;

    /* factorization by trial division using the primes list */
    for (size_t i = 0; i < primes_len && primes[i] * primes[i] <= n; ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) {
            size_t exp = 0;
            do {
                n /= p;
                ++exp;
            } while (n % p == 0);
            idx_t ip = prime_index(p);
            for (size_t k = 0; k < exp; ++k) {
                idxs[cnt++] = ip;
            }
            if (ip > mx)
                mx = ip;
        }
    }

    /* if remainder is a prime > last precomputed prime, extend sieve */
    if (n > 1) {
        uint64_t p = n;
        ensure_primes_up_to(p);
        idx_t ip = prime_index(p);
        idxs[cnt++] = ip;
        if (ip > mx)
            mx = ip;
    }

    /* sort indices (size is small, insertion sort works) */
    for (size_t i = 1; i < cnt; ++i) {
        idx_t key = idxs[i];
        size_t j = i;
        while (j > 0 && idxs[j - 1] > key) {
            idxs[j] = idxs[j - 1];
            --j;
        }
        idxs[j] = key;
    }

    /* median check */
    if (cnt & 1) { /* odd */
        return mx >= 2 * idxs[cnt / 2];
    } else { /* even */
        return mx >= idxs[cnt / 2 - 1] + idxs[cnt / 2];
    }
}

/* Memoised generation of the sequence. */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t next_candidate = 2;

    /* initialise primes once */
    if (primes == NULL)
        ensure_primes_up_to(5000000ULL);

    /* generate terms until we have at least n entries */
    while (terms_len < n) {
        if (satisfies((uint64_t)next_candidate)) {
            if (terms_cap == terms_len) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return terms[n - 1];
}
