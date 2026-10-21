/*
 * A247687  Numbers m with the property that the symmetric representation of
 *          sigma(m) has three parts of width one.
 * https://oeis.org/A247687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth smallest integer of the form 2^k * p^2 where p is
 *            a prime (≥ 3) and 0 ≤ k ≤ ⌊log2(p)⌋ − 1
 * Range:     u8 none, u16 none, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   cmp_seq_t, floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (The sequence is simply all such numbers sorted in increasing order.) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"          /* seq_t pow2(idx_t exp); */
#include "../utils/floor_log2_uint64.h" /* idx_t floor_log2_uint64(idx_t n); */
#include "../utils/cmp_seq_t.h"     /* int cmp_seq_t(const void*, const void*); */

seq_t a(idx_t n)
{
    /* Static cache of all generated terms */
    static bool initialized = false;
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;

    /* If not yet initialized, generate the terms up to a generous limit. */
    if (!initialized) {
        /* Upper bound for primes that suffices to generate far more than 10 000 terms. */
        const idx_t PRIMELIMIT = 200000;

        /* ---------- Simple sieve of Eratosthenes ---------- */
        size_t *prime_idx = (size_t *)malloc((size_t)PRIMELIMIT * sizeof(*prime_idx));
        uint8_t *sieve = (uint8_t *)calloc((size_t)PRIMELIMIT + 1, 1);
        if (!prime_idx || !sieve) return 0; /* allocation failure */

        size_t pi = 0;
        for (idx_t i = 2; i <= PRIMELIMIT; ++i) sieve[i] = 1;
        for (idx_t i = 2; i * i <= PRIMELIMIT; ++i)
            if (sieve[i])
                for (idx_t j = i * i; j <= PRIMELIMIT; j += i)
                    sieve[j] = 0;

        for (idx_t i = 3; i <= PRIMELIMIT; i += 2)
            if (sieve[i]) prime_idx[pi++] = i;
        /* Count of primes ≥3 */
        size_t primes_cnt = pi;

        /* Compute total number of terms to allocate array. */
        size_t term_cnt = 0;
        for (size_t i = 0; i < primes_cnt; ++i) {
            idx_t p = prime_idx[i];
            idx_t kmax = floor_log2_uint64(p) - 1;
            if (kmax < 0) continue;
            term_cnt += (size_t)(kmax + 1);
        }

        /* Allocate array for all terms. */
        seq_t *terms = (seq_t *)malloc(term_cnt * sizeof(seq_t));
        if (!terms) return 0; /* allocation failure */

        /* Generate terms. */
        size_t pos = 0;
        for (size_t i = 0; i < primes_cnt; ++i) {
            seq_t p = (seq_t)prime_idx[i];
            seq_t p2 = p * p;
            idx_t kmax = floor_log2_uint64((idx_t)p) - 1;
            for (idx_t k = 0; k <= kmax; ++k) {
                seq_t term = pow2(k) * p2;
                terms[pos++] = term;
            }
        }

        /* Sort the terms ascending. */
        qsort(terms, term_cnt, sizeof(seq_t), cmp_seq_t);

        /* Store in static cache and clean up temporary data. */
        free(prime_idx);
        free(sieve);
        cache   = terms;
        cache_size = term_cnt;
        initialized = true;
    }

    /* Return the requested term, if within bounds. */
    if (n == 0 || n > cache_size) return 0;
    return cache[n - 1];
}
