/*
 * A375402  Numbers whose maximal anti-runs of weakly increasing prime factors
 *          (with multiplicity) have distinct maxima.
 * https://oeis.org/A375402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers whose maximal anti‑runs of weakly increasing *
 *            prime factors (with multiplicity) have distinct maxima. * The
 *            function searches successive integers, keeps those that *
 *            satisfy the distinct‑maxima condition, and memoises the results.
 *            * It uses the helper factor_u64 for prime factoris...
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static bool has_distinct_maxima(uint64_t n)
{
    if (n == 1) return true;               /* 1 is accepted */

    uint64_t primes[128], exps[128];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    if (cnt == 0) return true;

    /* Build the multiplicity list of primes. */
    uint64_t fact[256];
    size_t fc = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        for (uint64_t e = 0; e < exps[i]; ++e) {
            fact[fc++] = p;
        }
    }

    /* Find maxima of maximal anti‑runs. */
    uint64_t maxima[256];
    size_t mc = 0;

    uint64_t last = fact[0];
    for (size_t i = 1; i < fc; ++i) {
        uint64_t cur = fact[i];
        if (cur == last) {               /* duplicate starts new run */
            maxima[mc++] = last;
            last = cur;                  /* new run starts with duplicate */
        } else {
            last = cur;                  /* continue current run */
        }
    }
    maxima[mc++] = last;                  /* last run */

    /* Ensure all maxima are distinct. */
    for (size_t i = 0; i + 1 < mc; ++i) {
        for (size_t j = i + 1; j < mc; ++j) {
            if (maxima[i] == maxima[j]) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* capacity in terms */
    static idx_t length = 0;      /* number of terms stored */
    static uint64_t candidate = 1;/* next integer to test */

    if (cache == NULL) {          /* lazy initialisation */
        cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        length = 0;
        candidate = 1;
    }

    /* Generate terms until we have at least n of them. */
    while (length < n) {
        if (candidate == 1 || has_distinct_maxima(candidate)) {
            cache[length++] = (seq_t)candidate;
        }
        ++candidate;
        if (length == cap) {      /* grow cache */
            cap <<= 1;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
    }

    return cache[n - 1];
}
