/*
 * A309304  Sums of two primes whose difference is semiprime.
 * https://oeis.org/A309304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A309304: sums of two primes whose difference is semiprime. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*------------------------------------------------------------------*/
/* Static state for the prime sieve and memoised sequence values. */
/*------------------------------------------------------------------*/
static idx_t          limit        = 200;       /* current sieve upper bound   */
static bool          *is_prime     = NULL;     /* primality flag array        */
static uint32_t      *spf          = NULL;     /* smallest‑prime‑factor array */
static idx_t         *prime_list   = NULL;     /* list of primes              */
static size_t         prime_count  = 0;         /* number of primes in list    */
static seq_t        *results      = NULL;     /* computed members of the seq */
static size_t         result_count = 0;         /* how many computed so far    */
static idx_t          current_s    = 2;         /* next integer to test        */
static size_t         result_cap   = 0;         /* capacity of results array   */

/*------------------------------------------------------------------*/
/* Rebuild the sieve and small‑prime‑factor tables up to new_limit. */
/*------------------------------------------------------------------*/
static void recompute_sieve(idx_t new_limit)
{
    free(is_prime);
    free(spf);
    free(prime_list);

    is_prime = (bool *)malloc((new_limit + 1) * sizeof(bool));
    spf      = (uint32_t *)malloc((new_limit + 1) * sizeof(uint32_t));
    if (!is_prime || !spf) abort();             /* out of memory */

    for (idx_t i = 0; i <= new_limit; ++i) {
        is_prime[i] = true;
        spf[i]      = 0;
    }
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i <= new_limit; ++i) {
        if (is_prime[i]) {
            spf[i] = (uint32_t)i;
            if (i <= new_limit / i) {
                for (idx_t j = i * i; j <= new_limit; j += i) {
                    if (is_prime[j]) {
                        is_prime[j] = false;
                    }
                    if (!spf[j]) spf[j] = (uint32_t)i;
                }
            }
        }
    }

    size_t cap = 128;
    prime_list = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!prime_list) abort();
    prime_count = 0;

    for (idx_t i = 2; i <= new_limit; ++i) {
        if (is_prime[i]) {
            if (prime_count == cap) {
                cap <<= 1;
                prime_list = (idx_t *)realloc(prime_list, cap * sizeof(idx_t));
                if (!prime_list) abort();
            }
            prime_list[prime_count++] = i;
        }
    }
}

/*------------------------------------------------------------------*/
/* Append a new term to the results array.                           */
/*------------------------------------------------------------------*/
static void push_result(seq_t val)
{
    if (result_count == result_cap) {
        result_cap = (result_cap == 0) ? 8 : result_cap << 1;
        results = (seq_t *)realloc(results, result_cap * sizeof(seq_t));
        if (!results) abort();
    }
    results[result_count++] = val;
}

/*------------------------------------------------------------------*/
/* Test whether x (x ≥ 2) is a semiprime using the spf array.       */
/*------------------------------------------------------------------*/
static bool is_semiprime(idx_t x)
{
    if (x < 2) return false;
    uint32_t a = spf[x];
    if (!a) return false;                /* safety check */
    idx_t b = x / a;
    if (b > limit) return false;         /* safety for extensions */
    return is_prime[b] && ((uint64_t)a * b == (uint64_t)x);
}

/*------------------------------------------------------------------*/
/* Main sequence function demanded by the build system.              */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        recompute_sieve(limit);           /* initial sieve */
        initialized = true;
    }

    /* Generate terms until we have at least n of them. */
    while (result_count < n) {

        /* Extend the sieve if the current number is beyond it. */
        if (current_s > limit) {
            idx_t new_limit = limit * 2;
            if (new_limit < current_s) new_limit = current_s;
            recompute_sieve(new_limit);
            limit = new_limit;
        }

        /* Check current_s for the defining property. */
        bool ok = false;
        for (size_t i = 0; i < prime_count && prime_list[i] <= current_s / 2; ++i) {
            idx_t p = prime_list[i];
            idx_t q = current_s - p;
            if (!is_prime[q]) continue;
            idx_t diff = q - p;
            if (is_semiprime(diff)) {
                ok = true;
                break;
            }
        }
        if (ok) push_result((seq_t)current_s);

        ++current_s;
    }

    return results[n - 1];
}
