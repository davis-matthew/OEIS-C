/*
 * A353059  Integers k such that the prime factorization of k uses digits from
 *          a proper subset of the digits of k.
 * https://oeis.org/A353059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   digit_counts, factor_u64, is_prime_small, seq_alloc_new_array,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353059: integers k such that the digits appearing in the
   prime factorisation of k (including the digits of the exponents)
   form a proper subset of the digits of k.  The function generates
   terms lazily, memoising them in a static cache. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/digit_counts.h"
#include "../utils/is_prime_small.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache of already-found terms */
    static seq_t *cache = NULL;      /* array of terms */
    static size_t cache_len = 0;     /* number of terms stored   */
    static size_t cache_cap = 0;     /* current array capacity   */
    static uint64_t next_candidate = 2; /* next integer to test     */

    /* Initialise cache once */
    if (!cache) {
        cache_cap = 128;
        cache = seq_alloc_new_array(cache_cap);
        if (!cache) exit(1);
    }

    /* Ensure the cache can hold at least n entries */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t k = next_candidate++;
        /* Skip primes – the sequence contains only composites */
        if (is_prime_small(k))
            continue;

        /* Digit set of k */
        unsigned char kcnt[10] = {0};
        digit_counts((seq_t)k, kcnt);
        bool has_k[10];
        for (int d = 0; d < 10; ++d) has_k[d] = kcnt[d] > 0;

        /* Prime factorisation */
        uint64_t primes[64], exps[64];
        size_t pcnt;
        factor_u64(k, primes, exps, &pcnt);

        /* Build the set of digits that appear in the factorisation
           (both primes and exponents) */
        bool used[10] = {0};
        for (size_t i = 0; i < pcnt; ++i) {
            unsigned char pcntx[10] = {0};
            digit_counts((seq_t)primes[i], pcntx);
            for (int d = 0; d < 10; ++d)
                if (pcntx[d]) used[d] = true;

            unsigned char ecnt[10] = {0};
            digit_counts((seq_t)exps[i], ecnt);
            for (int d = 0; d < 10; ++d)
                if (ecnt[d]) used[d] = true;
        }

        /* Check that used ⊂ has_k and that the sets are not equal */
        bool ok = true;
        int missing = 0;   /* digits present in k but not in factorisation */
        for (int d = 0; d < 10; ++d) {
            if (used[d] && !has_k[d]) { ok = false; break; }
            if (has_k[d] && !used[d]) ++missing;
        }
        if (!ok || missing == 0)
            continue;   /* not in the sequence */

        /* Store the term */
        cache[cache_len++] = (seq_t)k;
    }

    return cache[(size_t)n - 1];
}
