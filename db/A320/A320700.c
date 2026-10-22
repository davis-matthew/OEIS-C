/*
 * A320700  Odd numbers whose product of prime indices is a nonprime prime
 *          power (A246547).
 * https://oeis.org/A320700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A320700:  Odd numbers whose product of prime indices is a prime power
 *           of exponent at least 2.
 *
 * The function generates the sequence online, storing a cache of the
 * discovered terms.  The first term is 7, the second is 9, as in the
 * OEIS entry.
 *
 * Only standard C (and C++ with the usual library headers) and
 * the helper functions listed in the prompt are used.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Global data for sieve and sequence cache                            */
/* --------------------------------------------------------------------- */
static uint32_t *spf        = NULL;   /* smallest prime factor up to current_limit */
static uint32_t *prime_idx  = NULL;   /* prime -> index (1‑based) */
static uint32_t *primes     = NULL;   /* list of primes up to current_limit */
static size_t    primes_cnt = 0;      /* number of primes found */
static size_t    current_limit = 0;   /* current sieve limit */

static seq_t   *seq_cache  = NULL;   /* cache of sequence values   */
static idx_t   seq_size   = 0;        /* number of cached terms     */
static idx_t   seq_cap    = 0;        /* capacity of seq_cache      */

/* --------------------------------------------------------------------- */
/*  Helper: rebuild sieve up to 'limit' (upper‑bounded by size_t)       */
/* --------------------------------------------------------------------- */
static void rebuild_sieve(size_t limit)
{
    /* free old data */
    free(spf);
    free(prime_idx);
    free(primes);

    /* allocate new arrays */
    spf       = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    prime_idx = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    if (!spf || !prime_idx) return;             /* allocation failed */

    /* linear sieve to fill spf and prime_idx */
    size_t cap = 256;
    primes   = (uint32_t*)malloc(cap * sizeof(uint32_t));
    if (!primes) return;
    size_t pcnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            if (pcnt == cap) {
                cap <<= 1;
                primes = (uint32_t*)realloc(primes, cap * sizeof(uint32_t));
            }
            primes[pcnt] = (uint32_t)i;
            prime_idx[i] = (uint32_t)(pcnt + 1); /* 1‑based index */
            ++pcnt;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            uint32_t p = primes[j];
            uint64_t v = (uint64_t)i * p;
            if (v > limit) break;
            spf[v] = p;
            if (i % p == 0) break;
        }
    }
    primes_cnt = pcnt;
    current_limit = limit;
}

/* --------------------------------------------------------------------- */
/*  Helper: append a value to the cache                                */
/* --------------------------------------------------------------------- */
static void cache_append(seq_t val)
{
    if (seq_cap == 0) {
        seq_cap = 256;
        seq_cache = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    } else if (seq_size >= seq_cap) {
        seq_cap <<= 1;
        seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
    seq_cache[seq_size++] = val;
}

/* --------------------------------------------------------------------- */
/*  Test whether an odd number satisfies the definition                 */
/* --------------------------------------------------------------------- */
static bool accept_term(uint64_t n)
{
    /* list of prime factors of the product of indices */
    uint32_t primes_q[32];
    int       exp_q[32];
    size_t    qcnt = 0;

    /* factor n over odd primes */
    while (n > 1) {
        uint32_t p = spf[n];
        int exp_p = 0;
        while (n % p == 0) {
            n /= p;
            ++exp_p;
        }

        /* get the index of prime p */
        uint32_t idx = prime_idx[p];
        /* factor the index */
        uint32_t rem = idx;
        while (rem > 1) {
            uint32_t q = spf[rem];
            int exp_f = 0;
            while (rem % q == 0) {
                rem /= q;
                ++exp_f;
            }
            /* q appears exp_p * exp_f times in the product */
            size_t k = 0;
            while (k < qcnt && primes_q[k] != q) ++k;
            if (k == qcnt) {
                if (qcnt == 32) return false;   /* safety; should not happen */
                primes_q[qcnt] = q;
                exp_q[qcnt] = exp_p * exp_f;
                ++qcnt;
            } else {
                exp_q[k] += exp_p * exp_f;
            }
        }
    }

    /* the product is a prime power iff it has exactly one prime factor
       with exponent >= 2                                             */
    return (qcnt == 1 && exp_q[0] >= 2);
}

/* --------------------------------------------------------------------- */
/*  Main function to return the n-th term                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* one‑time initialisation */
    static bool inited = false;
    if (!inited) {
        rebuild_sieve(200000);      /* modest starting limit */
        inited = true;
    }

    /* generate terms until we reach the required index */
    static idx_t last_checked = 1;   /* start from 1 – first odd will be 3 */
    while (seq_size < n) {
        last_checked += 2;           /* look at the next odd number */
        if ((size_t)last_checked > current_limit) {
            size_t new_limit = current_limit ? current_limit * 2 : 200000;
            rebuild_sieve(new_limit);
        }
        if (accept_term((uint64_t)last_checked))
            cache_append((seq_t)last_checked);
    }

    return seq_cache[n - 1];
}
