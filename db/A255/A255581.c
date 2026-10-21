/*
 * A255581  Numbers prime(n) such that prime(n)^2 + prime(n+1)^2 -
 *          prime(n+2)^2 is prime.
 * https://oeis.org/A255581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 20, u32 n <= 23, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255581: prime(n) such that prime(n)^2 + prime(n+1)^2 - prime(n+2)^2 is prime.
 * The function returns the nth term (a(1)=13).  It builds primes on demand and caches the
 * discovered terms.  The implementation works for all builds (64‑bit or arbitrary‑precision)
 * by using the utilities supplied in the project. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"  /* build_primes_upto(limit, &array, &cnt) */
#include "../utils/is_prime_big.h"       /* bool is_prime_big(seq_t n) */
#include "../utils/seq_realloc.h"        /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

/* --------------------------------------------------------------------------- */
/*  Prime cache                                                          */
/* --------------------------------------------------------------------------- */
static uint64_t *primes      = NULL;   /* array of primes up to current limit        */
static size_t   prime_cnt    = 0;      /* number of primes stored                    */
static idx_t    prime_limit  = 0;      /* current sieve limit                        */
static size_t   prime_idx    = 0;      /* next triple index to evaluate              */

/* Ensure the prime list contains at least all primes ≤ new_limit.
 * The list is rebuilt with a larger allocation; the old array is freed. */
static void extend_primes(idx_t new_limit)
{
    if (!primes || new_limit > prime_limit) {
        uint64_t *new_primes = NULL;
        size_t   new_cnt = 0;
        build_primes_upto(new_limit, &new_primes, &new_cnt);
        free(primes);               /* free the old array (NULL ok) */
        primes      = new_primes;
        prime_cnt   = new_cnt;
        prime_limit = new_limit;
    }
}

/* --------------------------------------------------------------------------- */
/*  Result cache                                                          */
/* --------------------------------------------------------------------------- */
static seq_t   *seq    = NULL;   /* array of discovered sequence terms      */
static size_t   seq_cnt = 0;     /* number of terms currently stored        */
static size_t   seq_cap = 0;     /* allocated capacity for seq              */

/* Grow the result array to hold at least `need` elements.  The function uses
 * seq_realloc which is safe for both POD and arbitrary‑precision seq_t types. */
static void grow_seq_if_needed(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);
        if (!tmp) abort();        /* memory exhaustion – stop program */
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* --------------------------------------------------------------------------- */
/*  Sequence generation                                                   */
/* --------------------------------------------------------------------------- */
static void ensure_sequence(idx_t n)
{
    /* Ensure we have at least n terms.  The procedure grows the prime list and
     * examines successive triples until enough terms are found.  Calls are
     * idempotent – future calls for larger n will continue from where the
     * previous call left off. */
    while (seq_cnt < (size_t)n) {
        /* Initialise the prime list on first call. */
        if (prime_limit == 0) {
            extend_primes(100000);   /* initial sieve limit */
            prime_idx = 0;
        }

        /* Examine triples beginning at prime_idx while they fit in the current cache. */
        while (prime_idx + 2 < prime_cnt && seq_cnt < (size_t)n) {
            uint64_t p = primes[prime_idx];
            uint64_t q = primes[prime_idx + 1];
            uint64_t r = primes[prime_idx + 2];

            /* Compute p^2 + q^2 - r^2 in 128‑bit to avoid overflow. */
            __int128 v = (__int128)p * p + (__int128)q * q - (__int128)r * r;
            if (v > 0) {
                seq_t val = (seq_t)v;
                if (is_prime_big(val)) {
                    grow_seq_if_needed(seq_cnt + 1);
                    seq[seq_cnt++] = p;
                }
            }
            ++prime_idx;
        }

        /* If more terms are needed, extend the sieve and continue. */
        if (seq_cnt < (size_t)n) {
            idx_t new_limit = prime_limit * 2 + 10;
            extend_primes(new_limit);
            /* prime_idx remains unchanged – new primes are appended at the end. */
        }
    }
}

/* --------------------------------------------------------------------------- */
/*  Public interface                                                      */
/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined for n=0 – return 0 as a safe fallback */
    ensure_sequence(n);
    return seq[n - 1];
}
