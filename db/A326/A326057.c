/*
 * A326057  a(n) = gcd(A003961(n)-2n, A003961(n)-sigma(n)), where A003961(n)
 *          is fully multiplicative function with a(prime(k)) = prime(k+1).
 * https://oeis.org/A326057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( A003961(n)-2n , A003961(n)-sigma(n) )
 * Range:     u8 n <= 53, u16 n <= 7829, u32 n <= 9685, u64 n <= 9685,
 *            u128 n <= 9685, big any n
 * Helpers:   build_primes_upto, factor_u64, gcd, pow_uint, seq_mul, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003961(n) is the fully multiplicative function with a(prime(k)) = prime(k+1) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/gcd.h"            /* seq_t gcd(seq_t a, seq_t b)          */
#include "../utils/seq_mul.h"        /* seq_t seq_mul(seq_t a, seq_t b)      */
#include "../utils/pow_uint.h"       /* seq_t pow_uint(uint64_t base, idx_t exp) */
#include "../utils/sigma_of.h"       /* seq_t sigma_of(idx_t n)              */
#include "../utils/factor_u64.h"     /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out) */

static uint64_t *prime_list       = NULL;   /* list of primes up to current limit */
static size_t   prime_list_cnt    = 0;      /* number of primes in prime_list     */
static uint32_t *prime_index      = NULL;   /* prime_index[p] = position in prime_list (UINT32_MAX if not prime) */
static bool      primes_ready     = false;  /* whether prime data is initialized    */

/* Initialise (or enlarge) the prime tables up to a limit that guarantees
   that for all primes ≤ n, the next prime is also in the list. */
static void init_primes(idx_t n)
{
    idx_t limit = n * 2 + 100;   /* By Bertrand, next prime after p ≤ 2p ≤ 2n */
    if (limit < 10) limit = 10;

    /* If already built for a larger limit, keep it */
    if (primes_ready) {
        if (prime_list_cnt > 0 && prime_list_cnt > 0 && prime_list[prime_list_cnt - 1] >= limit)
            return;
    }

    /* Build primes */
    build_primes_upto(limit, &prime_list, &prime_list_cnt);

    /* Build index map for quick lookup of prime positions */
    prime_index = (uint32_t *)malloc((size_t)(limit + 2) * sizeof(uint32_t));
    for (size_t i = 0; i <= (size_t)(limit + 1); ++i)
        prime_index[i] = UINT32_MAX;  /* sentinel for "not a prime" */

    for (size_t i = 0; i < prime_list_cnt; ++i)
        prime_index[(size_t)prime_list[i]] = (uint32_t)i;

    primes_ready = true;
}

/* Main sequence function, valid for any n ≥ 1. */
seq_t a(idx_t n)
{
    /* Initialise prime tables (ensure they are large enough for n). */
    if (!primes_ready || (prime_list && prime_list[prime_list_cnt - 1] < n * 2 + 100))
        init_primes(n);

    /* Compute sigma(n). */
    seq_t sigma = sigma_of(n);

    /* Compute 2*n as seq_t. */
    seq_t two_n = (seq_t)n * (seq_t)2;

    /* Factor n. */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Compute B(n) = A003961(n) = product next_prime(p)^e */
    seq_t B = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = pr[i];
        uint32_t idx = prime_index[(size_t)p];
        uint64_t next_p = prime_list[(size_t)(idx + 1)];   /* next prime in the list */

        seq_t pow = pow_uint(next_p, (idx_t)ex[i]);
        B = seq_mul(B, pow);
    }

    /* Absolute differences: |B - 2n| and |B - sigma(n)| */
    seq_t diff1 = (B > two_n) ? (B - two_n) : (two_n - B);
    seq_t diff2 = (B > sigma)  ? (B - sigma ) : (sigma   - B);

    /* Return gcd of the two differences */
    return gcd(diff1, diff2);
}
