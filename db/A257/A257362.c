/*
 * A257362  Odd primes modulo which -163 is a square.
 * https://oeis.org/A257362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 3245, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the n-th odd prime p such that -163 is a quadratic
 * residue modulo p.  This is equivalent to requiring that there exists an
 * integer x with x² ≡ -163 (mod p).  For an odd prime p ≠ 163 the
 * condition is (−163|p) = 1, which is tested via Euler's criterion:
 *
 *     (-163)^( (p-1)/2 ) ≡ 1 (mod p)
 *
 * If p divides 163 (i.e. p = 163) the congruence holds trivially because
 * -163 ≡ 0 (mod 163).  The prime 2 is excluded because it is not odd.
 *
 * The function generates primes in ascending order using next_prime(), tests
 * each candidate, and caches the results.  Subsequent calls retrieve values
 * from the cache, making the function efficient for arbitrary calling
 * order.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"

static seq_t *prime_cache   = NULL;  /* cached primes satisfying the condition */
static size_t cache_len    = 0;       /* number of cached primes */
static size_t cache_cap    = 0;       /* allocated capacity */
static uint64_t last_checked = 1;    /* last prime examined */

seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n primes */
    while (cache_len < n)
    {
        /* Get the next prime greater than last_checked */
        uint64_t p = next_prime(last_checked + 1);
        last_checked = p;

        /* Skip the even prime 2 */
        if (p == 2)
            continue;

        bool accepted = false;

        /* Special case: p divides 163 */
        if (p == 163)
            accepted = true;
        else
        {
            /* Compute (-163) mod p as a positive residue */
            uint64_t mod163 = 163 % p;
            seq_t base = (seq_t)((mod163 == 0) ? 0 : p - mod163);

            /* Euler's criterion: base ^ ((p-1)/2) mod p should be 1 */
            seq_t res = mod_pow(base, (p - 1) / 2, (seq_t)p);
            if (res == (seq_t)1)
                accepted = true;
        }

        if (accepted)
        {
            /* Grow the cache if necessary */
            if (cache_len >= cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;          /* memory failure, return 0 */
                prime_cache = tmp;
                cache_cap   = new_cap;
            }
            prime_cache[cache_len++] = (seq_t)p;
        }
    }

    return prime_cache[n - 1];
}
