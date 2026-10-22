/*
 * A325126  a(1) = 1; a(n) = -Sum_{d|n, d<n} rad(n/d) * a(d), where rad =
 *          A007947.
 * https://oeis.org/A325126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(1) = 1; * a(n) = -Sum_{d|n, d<n} rad(n/d) * a(d) * where rad =
 *            A007947 (product of distinct prime factors). * The algorithm
 *            uses a fixed-size cache for all n up to MAX_N, * so that
 *            repeated calls (in any order) are fast. The cache array * is
 *            statically allocated, which works for both standard C...
 * Range:     i8 n <= 128, i16 n <= 9798, i32 n <= 9798, i64 n <= 9798,
 *            i128 n <= 9798, big any n
 * Helpers:   factor_u64, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/rad.h"

/* --------------------------------------------------------------------- */
/*  Cache for computed values                                            */
/* --------------------------------------------------------------------- */
#define MAX_N 1000000          /* safe upper bound for test data */

static seq_t a_cache[MAX_N + 1];
static bool a_computed[MAX_N + 1];

/* --------------------------------------------------------------------- */
/*  Recursive helper that handles caching                                 */
/* --------------------------------------------------------------------- */
static seq_t a_compute(idx_t n)
{
    /* If we have a cached value, return it immediately */
    if (n <= MAX_N && a_computed[n])
        return a_cache[n];

    seq_t result;

    if (n == 1) {
        result = 1;
    } else {
        /* Factorise n to generate all its divisors */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t pf_cnt;
        factor_u64(n, primes, exps, &pf_cnt);

        /* Temporary array for divisors : fits all divisor counts of n ≤ MAX_N */
        idx_t divisors[2048];
        size_t dlen = 1;
        divisors[0] = 1;

        for (size_t pi = 0; pi < pf_cnt; ++pi) {
            uint64_t p = primes[pi];
            uint64_t e = exps[pi];
            size_t old_len = dlen;
            uint64_t mult = 1;
            for (uint64_t k = 1; k <= e; ++k) {
                mult *= p;
                for (size_t j = 0; j < old_len; ++j)
                    divisors[dlen++] = (idx_t)(divisors[j] * mult);
            }
        }

        /* Sum over proper divisors */
        result = 0;
        for (size_t i = 0; i < dlen; ++i) {
            idx_t d = divisors[i];
            if (d == n)
                continue;
            seq_t rad_val = rad((idx_t)(n / d));
            seq_t a_d = a_compute(d);
            result += rad_val * a_d;
        }
        result = -result;
    }

    /* Cache the result if within bounds */
    if (n <= MAX_N) {
        a_cache[n] = result;
        a_computed[n] = true;
    }
    return result;
}

/* --------------------------------------------------------------------- */
/*  Public entry point                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The function is defined for n ≥ 1; behaviour for n < 1 is undefined */
    return a_compute(n);
}
