/*
 * A285993  Largest odd abundant number (A005231) equal to the product of n
 *          consecutive primes.
 * https://oeis.org/A285993
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula:
 *  a(n) = largest odd abundantly‑defined number that equals the product of
 *         n consecutive odd primes.
 *  The function locally builds an array of odd primes, computes the
 *  cumulative log‑sum of log((p+1)/p), then finds the right‑most
 *  window of length n whose cumulative log exceeds log(2).  The
 *  product of the primes in that window is returned.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* External helper (provided by the build system) */
#include "../utils/seq_mul.h"

/* ------------ constants and static data ---------------- */
#define PRIME_SIEVE_LIMIT 200000  /* sufficient for n up to ~20000 */

static unsigned char *sieve = NULL;
static uint32_t *odd_primes = NULL;
static size_t odd_prime_cnt = 0;
static long double *cum_log = NULL;
static bool initialized = false;

/* ------------ local helpers ---------------------------------- */

/* Build odd prime list and cumulative log array.  Must be called once. */
static void init_sequence(void)
{
    size_t i, j;
    /* 1. Simple sieve of Eratosthenes */
    sieve = (unsigned char*)calloc(PRIME_SIEVE_LIMIT + 1, 1);
    for (i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        sieve[i] = 1;
    for (i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i)
        if (sieve[i])
            for (j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                sieve[j] = 0;

    /* 2. Extract odd primes into array */
    odd_prime_cnt = 0;
    for (i = 3; i <= PRIME_SIEVE_LIMIT; i += 2)
        if (sieve[i])
            odd_prime_cnt++;

    odd_primes = (uint32_t*)calloc(odd_prime_cnt, sizeof(uint32_t));
    for (i = 3, j = 0; i <= PRIME_SIEVE_LIMIT; i += 2)
        if (sieve[i])
            odd_primes[j++] = (uint32_t)i;

    /* 3. Prefix sums of log((p+1)/p) */
    cum_log = (long double*)calloc(odd_prime_cnt + 1, sizeof(long double));
    cum_log[0] = 0.0L;
    for (i = 0; i < odd_prime_cnt; ++i) {
        long double p = (long double)odd_primes[i];
        cum_log[i + 1] = cum_log[i] + logl((p + 1.0L) / p);
    }

    /* Clean up temporary data */
    free(sieve);
    sieve = NULL;
    initialized = true;
}

/* ------------ the sequence function --------------------- */

seq_t a(idx_t n)
{
    /* n < 5 is undefined in the OEIS sequence */
    if (n < 5)
        return 0;

    if (!initialized)
        init_sequence();

    /* If the precomputed list is too short, return 0 (shouldn't happen) */
    if ((size_t)n > odd_prime_cnt)
        return 0;

    const size_t len = (size_t)n;
    const size_t total = odd_prime_cnt;

    /* Find the right‑most start position so that the window is abundant */
    size_t lo = 0, hi = total - len;
    size_t best_start = 0;
    const long double log2val = logl(2.0L);

    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        long double window_log = cum_log[mid + len] - cum_log[mid];
        if (window_log > log2val) {          /* window is abundant */
            best_start = mid;
            lo = mid + 1;
        } else {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }

    /* Compute the product of the selected window */
    seq_t prod = 1;
    for (size_t i = best_start; i < best_start + len; ++i)
        prod = seq_mul(prod, (seq_t)odd_primes[i]);

    return prod;
}
