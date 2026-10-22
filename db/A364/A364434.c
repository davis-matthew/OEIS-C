/*
 * A364434  a(1) = 12; for n >= 2, a(n) = least positive integer of the form
 *          prime(m)*prime(n-m)*prime(n) with m >= 1.
 * https://oeis.org/A364434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{1 <= m < n} prime(m) * prime(n-m) * prime(n) (with
 *            a(1) = 12)
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_vals   = NULL;   /* 1‑indexed: prime_vals[0] = 2        */
static size_t   prime_count   = 0;       /* number of primes stored             */

/* Ensure that prime_vals contains at least `need` primes.
   The sieve supplies all primes <= limit, which is chosen by the
   prime number theorem to guarantee at least that many primes. */
static void
ensure_primes(idx_t need)
{
    if (prime_count >= (size_t)need)
        return;

    /* If a previous allocation existed, free it. */
    free(prime_vals);
    prime_vals = NULL;

    idx_t limit;
    if (need < 6)
        limit = 15;                    /* trivially contains 6 primes   */
    else {
        double d = (double)need;
        /* Upper bound for the nth prime: n (log n + log log n) for n >= 6 */
        limit = (idx_t)(d * (log(d) + log(log(d)))) + 3;
    }

    size_t cnt;
    build_primes_upto(limit, &prime_vals, &cnt);

    /* In the unlikely event that the bound was too low, double it. */
    while (cnt < (size_t)need) {
        free(prime_vals);
        limit <<= 1;                    /* multiply by 2 */
        build_primes_upto(limit, &prime_vals, &cnt);
    }

    prime_count = cnt;
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1)                     /* by definition */
        return (seq_t)12;

    ensure_primes(n);               /* guarantees prime_vals[0..n-1] exist */

    seq_t best = 0;
    bool   first = true;

    for (idx_t m = 1; m < n; ++m) {   /* 1 <= m < n          */
        uint64_t p_m    = prime_vals[m - 1];
        uint64_t p_nm   = prime_vals[n - m - 1];
        uint64_t p_n    = prime_vals[n - 1];

        uint64_t prod64 = p_m * p_nm * p_n;
        seq_t    prod   = (seq_t)prod64;

        if (first || prod < best) {
            best = prod;
            first = false;
        }
    }
    return best;
}
