/*
 * A249761  Primes p such that there is prime q with sigma(q-1) = p.
 * https://oeis.org/A249761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   append_candidate, build_primes_upto, cmp_seq_t, is_prime_big,
 *            sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249761: primes p such that there exists a prime q with σ(q‑1)=p.
   Algorithm: iterate increasing primes q, compute σ(q‑1) using sigma_of.
   If σ(q‑1) is prime, record the resulting prime p.
   Results are cached, sorted and deduplicated so that a(n) returns the
   nth term (1‑based).  If n exceeds the number of terms found, 0 is
   returned.                                    */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"
#include "../utils/build_primes_upto.h"
#include "../utils/cmp_seq_t.h"

static seq_t *seq_cache = NULL;   /* sorted unique list of found primes p   */
static size_t cache_len = 0;      /* current number of stored terms          */
static size_t cache_cap = 0;      /* allocated capacity of seq_cache         */

static uint64_t *prime_list = NULL;   /* list of primes q                       */
static size_t prime_cnt = 0;          /* number of primes in prime_list          */
static idx_t   prime_limit = 0;       /* current sieve limit                     */
static idx_t   prime_index = 0;       /* next prime to process                   */

/* Ensure seq_cache contains at least n terms (1‑based).  Extends the
   list of primes and processes more q values as needed.  Returns
   when cache_len >= n.                                         */
static void ensure_cache(idx_t n)
{
    /* Grow prime list if necessary.  The list is re‑sieved at higher limits. */
    while (cache_len < (size_t)n) {
        /* If all primes at the current limit have been processed, extend.   */
        if (prime_index >= prime_cnt) {
            if (prime_limit == 0) prime_limit = 1000;
            else prime_limit <<= 1;           /* double the limit            */

            uint64_t *new_list;
            size_t new_cnt;
            build_primes_upto(prime_limit, &new_list, &new_cnt);

            free(prime_list);                 /* free old allocation        */
            prime_list = new_list;
            prime_cnt = new_cnt;
            /* prime_index remains valid – it points to the next prime
               in the new enlarged list.                                  */
        }

        /* Process the next prime q.                                   */
        idx_t q = prime_list[prime_index++];
        seq_t p = sigma_of(q - 1);                 /* p = σ(q-1)                 */

        /* If p is prime, record it.                                  */
        if (is_prime_big(p))
            append_candidate(&seq_cache, &cache_len, &cache_cap, p);
    }

    /* Sort cache and remove duplicates.  This keeps seq_cache in
       sorted unique order for all future calls.                     */
    if (cache_len > 0) {
        qsort(seq_cache, cache_len, sizeof(seq_t), cmp_seq_t);
        size_t uniq = 0;
        for (size_t i = 0; i < cache_len; ++i) {
            if (uniq == 0 || seq_cache[i] != seq_cache[uniq - 1])
                seq_cache[uniq++] = seq_cache[i];
        }
        cache_len = uniq;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – n==0 is undefined    */
    ensure_cache(n);
    if ((size_t)n <= cache_len)
        return seq_cache[n - 1];
    return 0;                     /* sequence exhausted (finite case)     */
}
