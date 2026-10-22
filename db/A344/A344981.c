/*
 * A344981  Number of Goldbach partitions of 2n into two even-indexed primes.
 * https://oeis.org/A344981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Goldbach partitions of 2*n whose two primes
 *            both have even prime indices (the list of primes starts with
 *            index 1 for the prime 2)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes.h"   /* build_primes(idx_t limit, idx_t **out, size_t *cnt) */

static idx_t  max_limit   = 0;      /* largest number for which primes are known   */
static idx_t *prime_list = NULL;   /* list of primes (0‑based)                    */
static size_t prime_cnt  = 0;      /* number of primes in prime_list              */
static uint32_t *prime_index = NULL; /* prime_index[p] == 0 → not prime,
                                      * else 1‑based prime index                  */

static seq_t *memo          = NULL;   /* cached a(k) values                       */
static idx_t  max_n_computed = 0;     /* highest n whose value is cached          */

/* Ensure all primes up to `limit` are available.  Rebuild tables when needed.
   This function is called only with increasing limits. */
static void
ensure_primes_up_to(idx_t limit)
{
    if (limit <= max_limit)
        return;

    free(prime_list);
    free(prime_index);
    prime_list  = NULL;
    prime_index = NULL;

    size_t cnt = 0;
    build_primes(limit, &prime_list, &cnt);
    prime_cnt = cnt;
    max_limit = limit;

    prime_index = (uint32_t*)calloc((size_t)limit + 1, sizeof(uint32_t));
    if (!prime_index)
        return;   /* silently fail; caller will return 0             */

    for (size_t i = 0; i < cnt; ++i) {
        idx_t p = prime_list[i];
        prime_index[p] = (uint32_t)(i + 1);   /* 1‑based index                     */
    }
}

/* Return the 1‑based index of prime `p`; 0 if `p` is not prime. */
static inline uint32_t
prime_idx(idx_t p)
{
    return (p <= max_limit) ? prime_index[p] : 0;
}

/* Count Goldbach partitions of 2*n into two primes with even indices. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Make sure all primes up to 2*n are known. */
    ensure_primes_up_to(2 * n);

    /* Grow the memoisation table if we have not yet computed up to `n`. */
    if (n > max_n_computed) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_memo = (seq_t*)realloc(memo, new_size * sizeof(seq_t));
        if (!new_memo)
            return (seq_t)0;   /* allocation failed – return zero as fallback */

        memo = new_memo;

        /* Compute a(k) for all k from max_n_computed+1 up to n. */
        for (idx_t k = max_n_computed + 1; k <= n; ++k) {
            seq_t count = 0;
            idx_t target = k + k;     /* 2*k */

            /* Iterate over primes p <= k.  prime_list is sorted. */
            for (size_t i = 0; i < prime_cnt; ++i) {
                idx_t p = prime_list[i];
                if (p > k)
                    break;

                idx_t q = target - p;
                if (q < p)    /* ensure p <= q and avoid double counting */
                    break;

                uint32_t idxp = prime_idx(p);
                uint32_t idxq = prime_idx(q);
                if (idxp == 0 || idxq == 0)
                    continue;   /* q not prime */

                /* Accept only primes with even indices. */
                if (!( (idxp & 1u) == 0u && (idxq & 1u) == 0u ))
                    continue;

                count += (seq_t)1;
            }
            memo[k] = count;
        }
        max_n_computed = n;
    }

    return memo[n];
}
