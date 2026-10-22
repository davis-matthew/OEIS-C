/*
 * A375170  Primes that can be expressed as the sum of a prime and twice a
 *          positive square.
 * https://oeis.org/A375170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A375170 – primes that can be expressed as p = q + 2·x²,
 *  where q and p are primes, x≥1.
 *
 *  The function scans prime numbers in ascending order, checks whether each
 *  prime has a representation of the required form, and stores them in a
 *  static cache.  Calls are independent of order, and the cache grows as
 *  needed.  Complexity is roughly linear in the number of primes examined.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Forward declarations of internal helper tables */
static seq_t *cond_cache   = NULL; /* primes that satisfy the condition  */
static size_t  cond_cnt    = 0;
static size_t  cond_cap    = 0;

static seq_t *prime_cache  = NULL; /* all primes seen so far            */
static size_t  prime_cnt   = 0;
static size_t  prime_cap   = 0;

static seq_t  last_checked = 2;     /* last candidate considered in the sieve */

/* Test whether x is prime using trial division by known primes.  */
static bool is_prime_trial(seq_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt && (seq_t)prime_cache[i] * (seq_t)prime_cache[i] <= x; ++i) {
        if (x % prime_cache[i] == 0) return false;
    }
    return true;
}

/* Extend the list of primes so that it contains all primes up to `limit`.
 * The extension is performed by scanning odd numbers only. */
static void grow_primes_up_to(seq_t limit)
{
    seq_t cand = last_checked + 1;
    if (cand <= 2) cand = 3;
    if (cand % 2 == 0) cand++;   /* ensure cand is odd */

    for (; cand <= limit; cand += 2) {
        if (is_prime_trial(cand)) {
            /* add to prime list */
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_cache = (seq_t *)realloc(prime_cache, prime_cap * sizeof(seq_t));
            }
            prime_cache[prime_cnt++] = cand;

            /* check condition for prime p = cand */
            seq_t p = cand;
            for (seq_t x = 1; (seq_t)2 * x * x < p; ++x) {
                seq_t tmp = p - (seq_t)2 * x * x;
                if (tmp < 2) break;
                if (is_prime_trial(tmp)) {
                    if (cond_cnt == cond_cap) {
                        cond_cap = cond_cap ? cond_cap * 2 : 16;
                        cond_cache = (seq_t *)realloc(cond_cache, cond_cap * sizeof(seq_t));
                    }
                    cond_cache[cond_cnt++] = p;
                    break;          /* stop after first representation found */
                }
            }
        }
    }
    last_checked = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for n=0, but safely return 0 */

    /* Grow the cache until we have at least n terms */
    while (cond_cnt < n) {
        /* Estimate an upper bound to search for new primes.
         * Heuristically grow the limit by a factor of 2 each time. */
        seq_t limit = last_checked * 2;
        if (limit < 10) limit = 10;  /* safety for first iteration */
        grow_primes_up_to(limit);
    }

    return cond_cache[n - 1];
}
