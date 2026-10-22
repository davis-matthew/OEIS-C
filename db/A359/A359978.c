/*
 * A359978  Prime numbers whose decimal representation can be split into two
 *          nonempty parts without leading 0, say x and y (in that order), and
 *          such that the concatenation of y and x yields a different prime
 *          number.
 * https://oeis.org/A359978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 2814, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A359978: Prime numbers whose decimal representation can be split into two
   non‑empty parts x and y (no leading zeros) such that the concatenation
   yx is a different prime.  a(1) = 13.  This implementation stores all
   terms found so far and grows the storage lazily. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Cache for previously computed sequence terms. */
static seq_t   *cache      = NULL;
static size_t  cache_cap   = 0;
static size_t  cache_size  = 0;

/* List of all primes encountered so far (used for primality tests). */
static idx_t   *prime_list = NULL;
static size_t  prime_cap   = 0;
static size_t  prime_count = 0;

/* Next integer to test for primality. */
static idx_t   next_candidate = 2;

/* --- Helper functions --------------------------------------------------- */

/* Ensure the prime list has room for at least 'need' primes. */
static void prime_list_ensure(size_t need)
{
    if (need <= prime_cap) return;
    size_t newcap = prime_cap ? prime_cap << 1 : 8;
    if (newcap < need) newcap = need;
    idx_t *tmp = (idx_t *)realloc(prime_list, newcap * sizeof(idx_t));
    if (!tmp) exit(1);
    prime_list = tmp;
    prime_cap  = newcap;
}

/* Test whether n is prime; if it is, add it to the prime list. */
static bool prime_candidate(idx_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= n; ++i)
        if (n % prime_list[i] == 0) return false;

    /* new prime */
    prime_list_ensure(prime_count + 1);
    prime_list[prime_count++] = n;
    return true;
}

/* Test primality of n using the current prime list (does not modify it). */
static bool is_prime_using_list(idx_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= n; ++i)
        if (n % prime_list[i] == 0) return false;
    return true;
}

/* Number of decimal digits of x (x>0). */
static int num_digits(idx_t x)
{
    int d = 0;
    do { x /= 10; ++d; } while (x);
    return d;
}

/* Return true if the prime p satisfies the OEIS condition. */
static bool has_split(idx_t p)
{
    if (p < 10) return false;           /* need at least two digits */

    int len = num_digits(p);

    /* pow10[i] = 10^i for i = 0 .. len-1 */
    idx_t pow10[20];
    pow10[0] = 1;
    for (int i = 1; i < len; ++i) pow10[i] = pow10[i - 1] * 10ULL;

    for (int split = 1; split < len; ++split) { /* split after 'split' digits from right */
        idx_t y = p % pow10[split];
        idx_t x = p / pow10[split];

        if (y / pow10[split - 1] == 0) continue;   /* leading zero in y */

        idx_t q = y * pow10[len - split] + x;
        if (q == p) continue;                      /* must be different */
        if (is_prime_using_list(q)) return true;    /* found a valid split */
    }
    return false;
}

/* --- Public API --------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but safe */

    /* Grow the cache until we have at least n terms. */
    while (cache_size < n) {
        /* Find the next prime integer. */
        idx_t p = next_candidate;
        for (;;) {
            if (prime_candidate(p)) break;       /* p is new prime … */
            ++p;                                 /* … otherwise try next */
        }
        next_candidate = p + 1;   /* start searching from the following integer */

        /* Check if this prime satisfies the conditions. */
        if (has_split(p)) {
            /* Append to cache. */
            if (cache_size == cache_cap) {
                size_t newcap = cache_cap ? cache_cap << 1 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_size++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
