/*
 * A256877  Numbers divisible by prime(d) for each digit d in their base-7
 *          representation, none of which may be zero.
 * https://oeis.org/A256877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence constructor for OEIS A256877
   Numbers whose base‑7 digits are all non‑zero and the integer
   itself is divisible by prime(d) for every digit d in its base‑7
   representation.  prime(1)=2, prime(2)=3, prime(3)=5,
   prime(4)=7, prime(5)=11, prime(6)=13.  Result stored in cache
   so each a(n) is found in O(n) time overall. */
#include <stdint.h>
#include <stdlib.h>

/* prime values for digits 1..6 in base 7 */
static const uint64_t primes[6] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL};

/* cache of computed terms */
static seq_t      *cache      = NULL;   /* array of seq_t terms */
static size_t      cache_cap  = 0;      /* current allocated capacity */
static idx_t       cached_cnt = 0;      /* number of terms cached so far */
static uint64_t    next_val   = 1;      /* next integer to test */

/* ensure cache has capacity for at least 'need' entries */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();          /* allocation failure */
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* add a new term to the cache */
static void add_cache_term(seq_t val)
{
    ensure_cache_cap(cached_cnt + 1);
    cache[cached_cnt++] = val;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset starts at 1 */

    /* return from cache if already computed */
    if (cached_cnt >= n)
        return cache[n-1];

    /* search for terms until we have the nth one */
    while (cached_cnt < n) {
        uint64_t cand = next_val++;
        uint64_t tmp = cand;
        unsigned mask = 0;
        int valid = 1;

        /* examine base‑7 digits */
        while (tmp > 0) {
            unsigned d = tmp % 7;
            if (d == 0) { valid = 0; break; }
            mask |= 1U << (d - 1);
            tmp /= 7;
        }

        if (!valid) continue;

        /* compute lcm of primes for digits present */
        uint64_t prod = 1;
        for (int i = 0; i < 6; ++i)
            if (mask & (1U << i))
                prod *= primes[i];

        /* check divisibility */
        if ( (cand % prod) == 0 ) {
            add_cache_term((seq_t)cand);
        }
    }

    return cache[n-1];
}
