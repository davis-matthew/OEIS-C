/*
 * A340932  Numbers whose least prime index is odd. Heinz numbers of integer
 *          partitions whose last part is odd.
 * https://oeis.org/A340932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 174, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340932: Numbers whose least prime index is odd.
   The function builds the list lazily using a linear sieve.  */
/* It uses static state and expands the sieve range as needed.
   The algorithm runs in O(m log log m) time for the
   maximum number m examined.  The sequence is valid for any n
   and does not depend on the call order.  */

/*=== include <stdlib.h> for malloc/realloc/calloc/free =====================*/
#include <stdlib.h>

/*=== static state for the cache and sieve =================================*/
static seq_t *cache      = NULL;  /* array of terms */
static size_t cache_cap  = 0;     /* cached capacity  */
static size_t cache_len  = 0;     /* number of cached terms */
static idx_t   upper     = 1;     /* largest number examined */

/*=== forward declaration of helper ---------------------------------------*/
static void append_term(seq_t val);

/*=== public function -------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* grow the cache until it contains at least n terms */
    while (cache_len < n) {
        /* increase the sieve limit (at least 100 to start) */
        idx_t new_limit = upper * 2;
        if (new_limit < 100) new_limit = 100;

        /* allocate auxiliary arrays for the sieve */
        idx_t *spf       = (idx_t *)calloc((size_t)new_limit + 1,
                                           sizeof(idx_t));
        idx_t *prime_idx = (idx_t *)calloc((size_t)new_limit + 1,
                                           sizeof(idx_t));

        /* dynamic array of primes */
        idx_t *primes     = NULL;
        size_t primes_cap = 0;
        size_t primes_len = 0;

        /* linear sieve to compute smallest prime factor and prime indices */
        for (idx_t i = 2; i <= new_limit; ++i) {
            if (spf[i] == 0) {               /* i is prime */
                spf[i] = i;
                prime_idx[i] = (idx_t)(primes_len + 1); /* 1‑based index */

                if (primes_len >= primes_cap) {
                    primes_cap = primes_cap ? primes_cap * 2 : 4;
                    primes = (idx_t *)realloc(primes,
                                               primes_cap * sizeof(idx_t));
                }
                primes[primes_len++] = i;
            }

            for (size_t j = 0; j < primes_len; ++j) {
                idx_t p = primes[j];
                idx_t ip = i * p;
                if (ip > new_limit) break;
                spf[ip] = p;
                if (p == spf[i]) break;
            }
        }

        /* collect terms in the new range [upper+1, new_limit] */
        for (idx_t x = upper + 1; x <= new_limit; ++x) {
            idx_t sp = spf[x];
            idx_t idx = prime_idx[sp];
            if (idx & 1U)            /* odd prime index */
                append_term((seq_t)x);
        }

        /* clean up */
        free(spf);
        free(prime_idx);
        free(primes);

        upper = new_limit;
    }

    return cache[n - 1];
}

/*=== helper: append a new term to the static cache ------------------------*/
static void append_term(seq_t val)
{
    if (cache_len >= cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 4;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}
