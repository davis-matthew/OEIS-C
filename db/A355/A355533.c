/*
 * A355533  Irregular triangle read by rows where row n lists the differences
 *          between adjacent prime indices of n; if n is prime(k), then row n
 *          is just (k).
 * https://oeis.org/A355533
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = k if n is prime(k); otherwise a(n) lists the differences
 *            between consecutive sorted prime indices of n (multiset of
 *            indices for each prime factor counted with multiplicity). The
 *            sequence is read row‑by‑row, starting with n = 2
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(idx_t n) can be called in any order.  It memoises
   previously computed values and uses a simple SPF sieve to factorise
   numbers.  The implementation works for all allowed seq_t and idx_t
   sizes. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Prime data ----------------------------------------------- */

static idx_t *spf = NULL;                 /* smallest prime factor of i */
static idx_t *prime_index_of = NULL;      /* index of prime p (1‑based) */
static idx_t primes_limit = 0;            /* largest number sieved so far */

/* Rebuild the SPF sieve up to 'limit'.  This creates arrays of size
   limit + 1 and fills spf[i] with the smallest prime factor of i (i
   itself for a prime).  For each prime p, prime_index_of[p] = counter. */
static void recompute_primes(idx_t limit)
{
    free(spf);
    free(prime_index_of);

    spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    prime_index_of = (idx_t *)calloc(limit + 1, sizeof(idx_t));
    if (!spf || !prime_index_of) {
        /* out of memory; leave arrays NULL so calls will fail gracefully */
        return;
    }

    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = 0;

    idx_t counter = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {           /* i is prime */
            spf[i] = i;
            prime_index_of[i] = ++counter;

            /* mark smallest prime factor for composites */
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    primes_limit = limit;
}

static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;
    recompute_primes(limit);
}

/* --- Sequence computation ------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Memoisation structures */
    static seq_t *cache = NULL;          /* array of computed a(k) */
    static idx_t cache_cap = 0;          /* size of allocated cache */
    static idx_t next_index = 2;         /* first index not yet cached
                                           (next on which to write) */
    static idx_t current_row = 2;        /* next n to generate */

    /* If value already cached, return it */
    if (cache && n < next_index)
        return cache[n];

    /* Generate rows until we have produced a(n) */
    while (next_index <= n) {
        idx_t m = current_row;

        /* Make sure we have prime data for this number */
        ensure_primes(m);

        /* Grow cache if necessary */
        if (next_index >= cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
            while (next_index > new_cap)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
            if (!tmp) { /* Allocation failure – release resources */
                free(cache);
                free(spf);
                free(prime_index_of);
                cache = NULL;
                cache_cap = 0;
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        if (spf[m] == m) {                /* m is prime */
            seq_t val = (seq_t)prime_index_of[m];
            cache[next_index++] = val;
        } else {                          /* composite row */
            /* Factor m using the SPF array. The indices of prime factors
               (counted with multiplicity) are produced in ascending order. */
            idx_t temp = m;
            idx_t indices[64];
            size_t ic = 0;

            while (temp > 1) {
                idx_t p = spf[temp];
                idx_t idx = prime_index_of[p];
                indices[ic++] = idx;
                temp /= p;
            }

            /* Emit all adjacent differences. */
            for (size_t i = 0; i + 1 < ic; ++i) {
                seq_t diff = (seq_t)(indices[i + 1] - indices[i]);
                cache[next_index++] = diff;
            }
        }
        ++current_row;
    }

    return cache[n];
}
