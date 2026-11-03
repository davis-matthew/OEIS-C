/*
 * A321898  Sum of coefficients of power sums symmetric functions in h(y) *
 *          Product_i y_i! where h is homogeneous symmetric functions and y is
 *          the integer partition with Heinz number n.
 * https://oeis.org/A321898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} (π(p)!)^e where π(p) = number of primes ≤ p
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 40, u64 n <= 72, u128 n <= 148,
 *            big any n
 * Validated: standard, pass, 67 terms through n = 988 (2026-11-01)
 * Helpers:   build_primes_upto, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/prime_pi.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

static uint64_t *g_primes = NULL;   /* array of primes up to current limit */
static size_t g_prime_cnt = 0;      /* number of primes in g_primes */
static idx_t g_max_limit = 0;       /* largest n for which primes have been built */

/* Build primes up to 'limit' and replace the global table. */
static void ensure_primes(idx_t limit)
{
    if (limit <= g_max_limit) return;
    if (g_primes) free(g_primes);
    build_primes_upto(limit, &g_primes, &g_prime_cnt);
    g_max_limit = limit;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* Ensure we have all primes up to at least n (needed for factorisation). */
    ensure_primes(n);

    /* Trial division using prime list. */
    for (size_t i = 0; i < g_prime_cnt && g_primes[i]*g_primes[i] <= temp; ++i)
    {
        uint64_t p = g_primes[i];
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            /* π(p) and its factorial. */
            seq_t pi  = prime_pi(p);
            seq_t fact = factorial((idx_t)pi);

            /* Multiply result by fact^exp. */
            for (int j = 0; j < exp; ++j)
                result *= fact;
        }
    }

    /* Remaining prime factor > sqrt(original n). */
    if (temp > 1)
    {
        uint64_t p = temp;
        seq_t pi  = prime_pi(p);
        seq_t fact = factorial((idx_t)pi);
        result *= fact;   /* exponent is 1 */
    }

    return result;
}
