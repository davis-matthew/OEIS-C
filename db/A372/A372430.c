/*
 * A372430  Positive integers k such that the distinct prime indices of k are
 *          a subset of the binary indices of k.
 * https://oeis.org/A372430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A372430: Positive integers k such that the distinct prime indices
   of k are a subset of the binary indices of k.
   Computation is performed by brute‑force search and trial division.
   The function remembers previously found terms and answers requests
   in any order. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---- Prime sieve --------------------------------------------------- */
static uint32_t *primes = NULL;      /* array of primes */
static size_t    prime_cnt = 0;      /* number of primes */
static size_t    prime_limit = 0;    /* largest prime generated */
static bool      primes_initialized = false;

/* Generate all primes <= limit (inclusive).  The array is stored in
   'primes' and its size in 'prime_cnt'.  If a larger limit is
   requested the sieve is recomputed. */
static void init_primes(size_t limit)
{
    if (primes_initialized && limit <= prime_limit) return;

    /* allocate a small boolean array */
    char *is_composite = (char *)calloc(limit + 1, 1);
    if (!is_composite) return;   /* memory failure */

    for (size_t i = 2; i * i <= limit; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_composite[j] = 1;

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++cnt;

    /* allocate and fill array */
    uint32_t *new_pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_pr) { free(is_composite); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) new_pr[idx++] = (uint32_t)i;

    free(is_composite);

    free(primes);          /* free old array if any */
    primes = new_pr;
    prime_cnt = cnt;
    prime_limit = limit;
    primes_initialized = true;
}

/* Binary search: return the 1‑based index of prime p if it is in
   the sieve, otherwise return 0. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint32_t val = primes[mid];
        if (val == (uint32_t)p) return mid + 1;
        if (val < (uint32_t)p) lo = mid + 1;
        else hi = mid;
    }
    return 0;    /* not found */
}

/* Check whether integer k satisfies the defining property. */
static bool passes(uint64_t k)
{
    if (k == 1) return true;   /* 1 has empty prime set and any binary set */

    uint64_t tmp = k;
    size_t last_prime = 0;     /* avoid duplicate primes */

    /* examine primes <= sqrt(tmp) */
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            if (p != last_prime) {   /* distinct prime factor */
                size_t idx = prime_index(p);
                if (idx == 0) return false;          /* should not happen */
                if (idx - 1 >= 64 || ((k >> (idx-1)) & 1) == 0)
                    return false;                       /* missing binary index */
                last_prime = p;
            }
            while (tmp % p == 0) tmp /= p;
        }
    }

    /* remaining prime factor > sqrt(original) */
    if (tmp > 1) {
        uint64_t p = tmp;
        /* if this prime is beyond current sieve enlarge it */
        if (p > prime_limit) init_primes((size_t)(p + 100));   /* small safety margin */
        size_t idx = prime_index(p);
        if (idx == 0) return false;          /* prime not in sieve */
        if (idx - 1 >= 64 || ((k >> (idx-1)) & 1) == 0)
            return false;
    }
    return true;
}

/* ---- Sequence storage ---------------------------------------------- */
static seq_t *seq_cache = NULL;   /* array of computed terms */
static size_t seq_len  = 0;        /* number of stored terms */
static size_t seq_cap  = 0;        /* allocated capacity */

/* Ensure capacity of the sequence array. */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) return; /* memory failure; we keep old array */
    seq_cache = new_arr;
    seq_cap   = new_cap;
}

/* ---- Public function ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined but silence unused warning */

    /* Initialize primes once.  A limit of 2 000 000 covers all primes
       needed for the first 10 000 terms.  Adjust if necessary. */
    if (!primes_initialized) init_primes(2000000);

    /* Lazy compute until we have at least n terms */
    static uint64_t next_candidate = 1;   /* next integer to test */
    while (seq_len < (size_t)n) {
        uint64_t candidate = next_candidate++;
        if (passes(candidate)) {
            ensure_seq_cap(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)candidate;
        }
    }
    return seq_cache[n-1];
}
