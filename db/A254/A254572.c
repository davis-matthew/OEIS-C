/*
 * A254572  Least multiple of n that is abundant or perfect (A023196).
 * https://oeis.org/A254572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest multiple of n that is abundant or perfect
 *            (sigma(m) ≥ 2m)
 * Range:     u8 n <= 22, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"

/* Limit for precomputation tables */
#define LIMIT 2000000

static bool init_done = false;
static seq_t *sigma = NULL;   /* σ(i) for 1 ≤ i ≤ LIMIT, zero‑initialized */
static seq_t *best  = NULL;   /* minimal abundant or perfect multiple of i */

/* Compute σ(i) for all i ≤ LIMIT and the best array */
static void compute_sieve(void)
{
    size_t i, j;
    /* Allocate zero‑initialised arrays */
    sigma = seq_alloc(LIMIT + 1);
    best  = seq_alloc(LIMIT + 1);

    /* Compute divisor sums by a classic divisor sieve */
    for (i = 1; i <= LIMIT; ++i) {
        seq_t add = (seq_t)i;
        for (j = i; j <= LIMIT; j += i) {
            sigma[j] += add;
        }
    }

    /* For each i find the first multiple that is abundant or perfect */
    for (i = 1; i <= LIMIT; ++i) {
        for (j = i; j <= LIMIT; j += i) {
            if (sigma[j] >= (seq_t)2 * (seq_t)j) {
                best[i] = (seq_t)j;
                break;
            }
        }
    }
    init_done = true;
}

/* Compute σ(m) for a single integer m (m ≤ 2^64) using trial division.
   Works for any seq_t size since intermediate products are performed in seq_t.
*/
static seq_t sigma_of(uint64_t m)
{
    seq_t result = 1;
    uint64_t tmp = m;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            /* For factor p^e, accumulate sum = 1 + p + … + p^e */
            seq_t term = 1;          /* p^0 */
            seq_t p_pow = 1;         /* current power of p */
            while (tmp % p == 0) {
                tmp /= p;
                p_pow *= (seq_t)p;
                term += p_pow;
            }
            result *= term;
        }
    }
    /* Remaining prime factor, if any */
    if (tmp > 1) {
        result *= ((seq_t)1 + (seq_t)tmp);
    }

    return result;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (!init_done) {
        compute_sieve();
    }

    /* Return precomputed value if within range */
    if ((idx_t)n <= LIMIT) {
        return best[n];
    }

    /* For n > LIMIT, perform a direct search using the σ‑function above */
    seq_t n_seq = (seq_t)n;
    seq_t k     = 1;
    seq_t m;

    for (;;) {
        m = n_seq * k;
        seq_t sigma_m = sigma_of((uint64_t)m);
        if (sigma_m >= (seq_t)2 * m) {
            return m;
        }
        ++k;
    }
}
