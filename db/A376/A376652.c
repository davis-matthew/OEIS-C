/*
 * A376652  Points of downward concavity in the sequence of composite numbers
 *          (A002808).
 * https://oeis.org/A376652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the index i of a strict descent in the first differences
 *            of the composite‑number sequence. Equivalently, a(n) = i such
 *            that c_{i+1}-c_i > c_{i+2}-c_{i+1}, where c_k denotes the k‑th
 *            composite number (offset 1)
 * Range:     u8 n <= 45, u16 n <= 6251, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *prime_arr = NULL;   /* prime table for primality tests   */
static size_t   prime_cnt  = 0;      /* number of primes stored          */
static size_t   prime_cap  = 0;      /* allocated capacity for prime_arr */

static seq_t *cache      = NULL;    /* cached results of a(n)          */
static size_t cache_size = 0;       /* current number of cached terms   */
static size_t cache_cap  = 0;       /* allocated capacity for cache    */

static uint64_t candidate      = 4;  /* next integer to test            */
static uint64_t last_comp      = 0;  /* most recent composite number    */
static seq_t    prev_diff      = 0;  /* previous first difference       */
static bool     prev_diff_def  = false; /* has prev_diff been set?        */
static idx_t    comp_idx       = 0;  /* index of most recent composite */

/* Grow the prime table to at least 'new_cap' entries. */
static void grow_primes(void)
{
    if (prime_cnt >= prime_cap) {
        prime_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
        prime_arr = (uint64_t *)realloc(prime_arr, prime_cap * sizeof(uint64_t));
    }
}

/* Store a newly discovered prime. */
static void add_prime(uint64_t p)
{
    grow_primes();
    prime_arr[prime_cnt++] = p;
}

/* Grow the cache array to at least 'new_cap' entries. */
static void grow_cache(void)
{
    if (cache_size >= cache_cap) {
        cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Append a value to the cache. */
static void cache_push(seq_t val)
{
    grow_cache();
    cache[cache_size++] = val;
}

/* Simple trial division primality test using known primes. */
static bool is_composite(uint64_t n)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p * p > n) break;
        if (n % p == 0) return true;
    }
    return false; /* n is prime */
}

/* Initialise static data once. */
static void init_once(void)
{
    add_prime(2);
    add_prime(3);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, guard just in case */

    static bool initialized = false;
    if (!initialized) {
        init_once();
        initialized = true;
    }

    /* Generate composites until we have at least n records. */
    while (cache_size < (size_t)n) {

        /* Find next composite number, expanding prime table as needed. */
        while (true) {
            bool comp = is_composite(candidate);
            if (comp) break;           /* found composite */
            add_prime(candidate);      /* candidate is prime */
            ++candidate;
        }

        /* Process the found composite. */
        ++comp_idx;                     /* comp_idx is 1‑based index */

        if (comp_idx == 1) {            /* first composite */
            last_comp = candidate;
        } else if (comp_idx == 2) {     /* first difference */
            prev_diff = (seq_t)(candidate - last_comp);
            prev_diff_def = true;
            last_comp = candidate;
        } else {                        /* subsequent differences */
            seq_t new_diff = (seq_t)(candidate - last_comp);
            if (prev_diff_def && new_diff < prev_diff) {
                /* Descent at position comp_idx - 2 */
                cache_push((seq_t)(comp_idx - 2));
            }
            prev_diff = new_diff;
            last_comp = candidate;
        }

        ++candidate;                    /* advance for next loop */
    }

    return cache[n - 1];
}
