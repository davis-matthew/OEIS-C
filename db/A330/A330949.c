/*
 * A330949  Odd nonprime numbers whose prime indices are not all prime
 *          numbers.
 * https://oeis.org/A330949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd composite n whose prime‑index multiset is not all
 *            prime * Implementation: generate primes up to a fixed bound
 *            (4×10⁵), build a map * from prime to its 1‑based index, and then
 *            iterate odd numbers, testing * each candidate by trial division.
 *            Candidates whose prime indices are * *not* all...
 * Range:     u8 n <= 46, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   append_candidate, is_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_primes.h"
#include "../utils/is_prime.h"
#include "../utils/append_candidate.h"

static idx_t *prime_list = NULL;          /* list of primes up to LIMIT */
static size_t prime_count = 0;
static idx_t *prime_to_index = NULL;     /* prime -> its 1‑based index (0 if not prime) */
static seq_t *cache = NULL;              /* cache of computed a(n) */
static size_t cache_size = 0;            /* number of cached terms */
static size_t cache_cap = 0;             /* allocated cache capacity */
static seq_t last_candidate = 0;         /* last odd number that qualified */
static bool initialized = false;

/* Precompute primes, the index map, and initialize the cache. */
static void init(void)
{
    if (initialized) return;

    const idx_t LIMIT = 400000;          /* sufficient for n ≤ 10 000 */
    sieve_primes(LIMIT, &prime_list, &prime_count);

    prime_to_index = (idx_t*)calloc((size_t)(LIMIT + 1), sizeof(idx_t));
    for (size_t i = 0; i < prime_count; ++i)
        prime_to_index[(size_t)prime_list[i]] = (idx_t)(i + 1); /* 1‑based */

    cache_cap = 128;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    cache_size = 0;
    last_candidate = 0;
    initialized = true;
}

/* Return the n-th term (n ≥ 1).  The function is memoised. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0 */

    init();

    /* Return a cached value if already computed. */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* Start searching from the last candidate or from 3. */
    seq_t cand = (cache_size == 0) ? (seq_t)3 : last_candidate + 2;
    size_t found = cache_size;
    while (found < n) {
        seq_t temp = cand;
        bool composite = false;
        bool allprimeIndices = true;

        /* Trial division by precomputed primes. */
        for (size_t i = 0; i < prime_count &&
                           (seq_t)prime_list[i] * (seq_t)prime_list[i] <= temp;
             ++i)
        {
            idx_t p = prime_list[i];
            if (temp % (seq_t)p == 0) {
                composite = true;
                idx_t idx_p = prime_to_index[(size_t)p];
                if (!is_prime(idx_p)) {
                    allprimeIndices = false;
                    break; /* composite index found */
                }
                while (temp % (seq_t)p == 0) temp /= p;
            }
        }

        /* Process any remaining prime factor > sqrt(cand). */
        if (composite && allprimeIndices == false) {
            /* Candidate already has a composite index; it qualifies. */
            append_candidate(&cache, &cache_size, &cache_cap, cand);
            ++found;
            last_candidate = cand;
        } else if (composite && temp > (seq_t)1) {
            /* Residual prime factor. */
            idx_t idx_p = prime_to_index[(size_t)temp];
            if (!is_prime(idx_p)) {
                append_candidate(&cache, &cache_size, &cache_cap, cand);
                ++found;
                last_candidate = cand;
            }
        }
        /* If composite == false, cand is prime; skip. */

        cand += (seq_t)2;   /* next odd number */
    }

    return cache[n - 1];
}
