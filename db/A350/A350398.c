/*
 * A350398  Numbers k such that for all pairs of primes p,q with p+q = 2*k,
 *          p*q mod 2*k is prime.
 * https://oeis.org/A350398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the OEIS sequence A350398.
   Definition: a(n) is the nth integer k (with a(1)=1) such that for
   every pair of primes p,q with p+q = 2*k, the value (p*q) mod (2*k)
   is itself prime.  The function builds the sequence on demand using
   a sieve of Eratosthenes and caches results for repeated calls.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Sieve state ------------------------------------------------------- */
static bool *is_prime_arr = NULL;   /* primality flag array */
static uint64_t *prime_list   = NULL;   /* list of primes */
static size_t   prime_count   = 0;      /* number of primes in list */
static size_t   prime_limit   = 0;      /* highest number sieved (inclusive) */

/* Ensure we have a prime sieve covering all numbers up to n (inclusive)  */
static void
ensure_sieve (uint64_t n)
{
    if (n <= prime_limit)
        return;

    /* Grow the limit exponentially to avoid frequent reallocations.   */
    uint64_t new_limit = prime_limit ? prime_limit : 2;
    while (new_limit < n)
        new_limit <<= 1;

    /* Allocate new arrays. */
    bool   *new_arr   = (bool *)malloc((new_limit + 1) * sizeof(bool));
    if (!new_arr) abort();
    for (uint64_t i = 0; i <= new_limit; ++i)
        new_arr[i] = true;
    new_arr[0] = new_arr[1] = false;

    /* Sieve */
    for (uint64_t i = 2; i * i <= new_limit; ++i) {
        if (new_arr[i]) {
            for (uint64_t j = i * i; j <= new_limit; j += i)
                new_arr[j] = false;
        }
    }

    /* Collect primes. */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= new_limit; ++i)
        if (new_arr[i]) ++cnt;
    uint64_t *new_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_list) abort();
    size_t idx = 0;
    for (uint64_t i = 2; i <= new_limit; ++i)
        if (new_arr[i]) new_list[idx++] = i;

    /* Replace old arrays.  */
    free(is_prime_arr);
    free(prime_list);
    is_prime_arr = new_arr;
    prime_list   = new_list;
    prime_count  = cnt;
    prime_limit  = new_limit;
}

/* ------------------------------------------------------------------ */
/*  Sequence state ---------------------------------------------------- */
static seq_t   *seq        = NULL;     /* stored sequence values          */
static size_t  seq_cap    = 0;          /* allocated capacity              */
static size_t  seq_len    = 0;          /* number of terms known so far    */
static idx_t   last_k     = 0;          /* last k that has been tested     */

/* Ensure working capacity for the seq array. */
static void
ensure_seq_cap (size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_seq) abort();
    seq = new_seq;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function ------------------------------------------- */
seq_t
a (idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0, but guard anyway */

    /* Extend sequence until we have at least n terms. */
    while (seq_len < (size_t)n)
    {
        /* Test the next k (strictly increasing). */
        ++last_k;

        /* We need primes up to 2*last_k for both p+q checks and
           for modular arithmetic. */
        ensure_sieve (last_k * 2);

        uint64_t m = last_k * 2;    /* value 2*k */
        bool ok = true;

        /* Iterate over all primes p <= last_k.
           For each, q = m - p is the complementary prime. */
        for (size_t i = 0; i < prime_count && prime_list[i] <= last_k; ++i)
        {
            uint64_t p = prime_list[i];
            uint64_t q = m - p;
            if (q < 2) continue;           /* q must be a prime >= 2 */
            if (q > prime_limit) {          /* extend sieve if needed */
                ensure_sieve (q);
            }
            if (!is_prime_arr[q]) continue; /* skip if q not prime */

            /* Compute (p*q) mod m safely. */
            uint64_t prod = ((unsigned __int128)p * q) % m;

            /* prod < m <= prime_limit, so primality test via array. */
            if (!is_prime_arr[prod]) {
                ok = false;
                break;
            }
        }

        /* If condition satisfied, record k. */
        if (ok) {
            ensure_seq_cap (seq_len + 1);
            seq[seq_len++] = (seq_t)last_k;
        }
    }

    return seq[n - 1];
}
