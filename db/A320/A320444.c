/*
 * A320444  Number of uniform hypertrees spanning n vertices.
 * https://oeis.org/A320444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n-1} (n-1)!/(d!*((n-1)/d)!) * (n/d)^((n-1)/d-1),
 *            a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute the ratio of factorials using prime exponents, and
 * evaluate the rational power as (n/d)^(k) = n^k / d^k. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"

/* --------------------------------------------------------------------- */
/* Prime list utilities (thread‑safe, lazily constructed).              */
/* --------------------------------------------------------------------- */
static uint64_t *prime_list = NULL;
static size_t    prime_count = 0;
static idx_t     prime_limit = 0;

static void update_prime_cache(idx_t need)
{
    if (need <= prime_limit) return;
    uint64_t *tmp = NULL;
    size_t   tmpcnt = 0;
    build_primes_upto(need, &tmp, &tmpcnt);

    free(prime_list);
    prime_list  = tmp;
    prime_count = tmpcnt;
    prime_limit = need;
}

/* --------------------------------------------------------------------- */
/* Velocity of p in m! (Legendre's formula).                            */
/* --------------------------------------------------------------------- */
static idx_t exponent_in_factorial(idx_t m, idx_t p)
{
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

/* --------------------------------------------------------------------- */
/* Human‑friendly computation of the factorial ratio                    */
/* m!/(d!*((m/d)!)).                                                     */
/* --------------------------------------------------------------------- */
static seq_t ratio_factorial(idx_t m, idx_t d)
{
    seq_t prod = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > m) break;
        idx_t e = exponent_in_factorial(m, p)
                - exponent_in_factorial(d, p)
                - exponent_in_factorial(m / d, p);
        if (e > 0) {
            prod *= seq_pow((seq_t)p, e);
        }
    }
    return prod;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                  */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Base cases from the OEIS data */
    if (n <= 2) return 1;

    /* m = n-1; we sum over all divisors d of m */
    idx_t m = n - 1;

    /* Ensure we have a prime list covering up to m */
    update_prime_cache(m);

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d != 0) continue;          /* d is not a divisor */

        /* -----------------------------------------------------------------
         * First divisor (d)
         * ----------------------------------------------------------------- */
        {
            idx_t k = m / d - 1;            /* exponent for power term   */
            seq_t term = ratio_factorial(m, d);

            if (k > 0) {
                seq_t num = seq_pow((seq_t)n, k);   /* n^k  */
                seq_t den = seq_pow((seq_t)d, k);   /* d^k  */
                term = term * num / den;            /* exact division    */
            }
            sum += term;
        }

        /* -----------------------------------------------------------------
         * Second divisor (m/d) if distinct from d
         * ----------------------------------------------------------------- */
        idx_t d2 = m / d;
        if (d2 != d) {
            idx_t k = m / d2 - 1;
            seq_t term = ratio_factorial(m, d2);

            if (k > 0) {
                seq_t num = seq_pow((seq_t)n, k);
                seq_t den = seq_pow((seq_t)d2, k);
                term = term * num / den;
            }
            sum += term;
        }
    }

    return sum;
}
