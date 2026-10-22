/*
 * A364537  Heinz numbers of integer partitions where some part is the
 *          difference of two consecutive parts.
 * https://oeis.org/A364537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A364537:
   Heinz numbers of integer partitions where some part equals a difference
   of two consecutive parts.  The function scans integers starting from 2
   and keeps the smallest numbers that satisfy the property; it stores
   seen values in a cache so that any n ≥ 1 may be answered in O(1)
   after the required range has been explored. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Global data for the smallest‑prime‑factor sieve */
static uint32_t *spf       = NULL;   /* spf[i] = smallest prime factor of i   */
static uint32_t *prime_idx = NULL;   /* prime_idx[p] = index of prime p (1‑based) */
static size_t   sievelimit = 0;      /* current sieve limit, inclusive          */

/* Cache for the sequence values */
static seq_t   *cache       = NULL;
static size_t   cache_len   = 0;       /* number of values stored              */
static size_t   cache_cap   = 0;       /* allocated cache capacity              */

/* Highest integer examined so far */
static idx_t    current_max = 1;        /* 1 is not a term of the sequence     */

/* ------------------------------------------------------------------ */
/* Build a smallest‑prime‑factor sieve up to 'limit' (inclusive).      */
/* The sieve also records the prime index of each prime.               */
static void build_sieve(size_t limit)
{
    uint32_t *new_spf, *new_pid;
    size_t i, j;

    new_spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    new_pid = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!new_spf || !new_pid) {
        /* Allocation failure – abort; the environment will raise an error. */
        exit(EXIT_FAILURE);
    }

    for (i = 0; i <= limit; ++i) {
        new_spf[i] = (uint32_t)i;
    }
    if (limit >= 1) {
        new_spf[1] = 1;
    }

    /* Standard sieve: fill the spf array. */
    for (i = 2; i <= limit; ++i) {
        if (new_spf[i] == (uint32_t)i) {          /* i is prime          */
            /* Avoid overflow in i*i by checking i <= limit / i */
            if (i <= limit / i) {
                for (j = i * i; j <= limit; j += i) {
                    if (new_spf[j] == (uint32_t)j) {
                        new_spf[j] = (uint32_t)i;
                    }
                }
            }
        }
    }

    /* Build prime indices.  prime_idx[p] is 0 for non‑primes.      */
    uint32_t idx = 0;
    for (i = 0; i <= limit; ++i) {
        if (i >= 2 && new_spf[i] == (uint32_t)i) {
            ++idx;
            new_pid[i] = idx;        /* 1‑based prime index */
        } else {
            new_pid[i] = 0;
        }
    }

    /* Replace old tables.  The old ones are freed automatically here. */
    free(spf);
    free(prime_idx);
    spf        = new_spf;
    prime_idx  = new_pid;
    sievelimit = limit;
}

/* ------------------------------------------------------------------ */
/* Determine whether integer m satisfies the A364537 property.        */
/* A prime factorization is translated into partition parts (prime   */
/* indices), which are then tested for the required difference      */
/* property.  The function assumes that spf and prime_idx are valid   */
/* for all values up to at least m.                                   */
static bool good_integer(idx_t m)
{
    if (m < 6) return false;   /* the smallest term is 6 */

    /* Collect the parts (prime indices).  A 64‑entry buffer is sufficient
       for all numbers likely to appear in the first few thousand terms.     */
    uint32_t parts[64];
    size_t len = 0;
    idx_t rem  = m;

    while (rem > 1) {
        uint32_t p = spf[rem];
        uint32_t pi = prime_idx[p];
        if (pi == 0) return false;        /* should not happen */
        /* Add pi for each occurrence of the prime factor.   */
        while (rem % p == 0) {
            if (len >= 64) return false;   /* safety: buffer overflow */
            parts[len++] = pi;
            rem /= p;
        }
    }

    if (len < 2) return false;   /* partition must have at least two parts */

    /* Sort parts ascending (insertion sort – small array). */
    for (size_t i = 1; i < len; ++i) {
        uint32_t key = parts[i];
        size_t j = i;
        while (j > 0 && parts[j - 1] > key) {
            parts[j] = parts[j - 1];
            --j;
        }
        parts[j] = key;
    }

    /* Check for existence of a part equal to a positive difference. */
    for (size_t i = 1; i < len; ++i) {
        uint32_t diff = parts[i] - parts[i - 1];
        if (diff == 0) continue;           /* zero difference is irrelevant */
        for (size_t k = 0; k < len; ++k) {
            if (parts[k] == diff) return true;
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Main function required by the harness.  It returns the n-th term
   of the sequence.  The cache is expanded lazily as needed.       */
seq_t a(idx_t n)
{
    /* Ensure the sieve is built.  The initial limit is 2 000 000,
       which is ample for early terms; the limit is doubled if
       necessary when searching further. */
    if (!spf) {
        build_sieve(2000000);
    }

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        ++current_max;                      /* next integer to test      */
        if ((size_t)current_max > sievelimit) {
            size_t new_limit = sievelimit * 2;
            if (new_limit < (size_t)current_max) new_limit = current_max;
            build_sieve(new_limit);
        }

        if (good_integer(current_max)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {                  /* allocation failure */
                    /* The environment will treat this as an error. */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)current_max;
        }
    }
    return cache[n - 1];
}
