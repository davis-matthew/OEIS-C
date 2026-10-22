/*
 * A326713  Numbers m that are neither arithmetic (A003601) nor RMS numbers
 *          (A140480).
 * https://oeis.org/A326713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 43, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, floor_sqrt, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th integer m (offset 1) such that
   neither arithmetic mean σ(m)/τ(m) nor RMS value sqrt(σ₂(m)/τ(m))
   is an integer (OEIS A326713).  It tests consecutive m by
   factorisation with trial division over a growing prime table
   and evaluates the necessary divisor sums multiplicatively.
   The algorithm is O(m log log m) and works for the
   required ranges. */

#include <stdbool.h>
#include <stdlib.h>

/* Utility headers (provided by the build system) */
#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"
#include "../utils/big_pow.h"
#include "../utils/int_sqrt.h"

/* Static cache of primes for factorisation */
static uint64_t *prime_arr   = NULL;
static size_t    prime_cnt   = 0;
static idx_t     prime_limit = 0;

/* Ensure that prime_arr contains every prime ≤ limit (inclusive). */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;
    free(prime_arr);
    build_primes_upto(limit, &prime_arr, &prime_cnt);
    prime_limit = limit;
}

/* Return the n‑th term of A326713.  n ≥ 1. */
seq_t
a(idx_t n)
{
    idx_t found = 0;   /* How many good numbers have been seen */
    idx_t m     = 2;   /* Candidate integer, start after 1 */

    while (1) {
        idx_t temp = m;

        /* Prime table must reach at least √m. */
        ensure_primes(int_sqrt((idx_t)m) + 1);

        seq_t sigma  = 1;   /* σ(m) = ∏ (p^(e+1)-1)/(p-1) */
        seq_t sigma2 = 1;   /* σ₂(m) = ∏ (p^(2(e+1))-1)/(p²-1) */
        seq_t tau    = 1;   /* τ(m) = ∏ (e+1) */

        size_t i = 0;
        while (i < prime_cnt && (idx_t)prime_arr[i] * prime_arr[i] <= temp) {
            uint64_t p = prime_arr[i];
            if (temp % p != 0) { ++i; continue; }

            idx_t e = 0;
            while (temp % p == 0) { temp /= p; ++e; }

            seq_t p_pow  = big_pow((seq_t)p, (idx_t)(e + 1));
            seq_t term   = (p_pow - 1) / ((seq_t)p - 1);
            sigma  *= term;

            seq_t p2    = (seq_t)p * (seq_t)p;
            seq_t p_pow2 = big_pow(p2, (idx_t)(e + 1));
            seq_t term2 = (p_pow2 - 1) / (p2 - 1);
            sigma2 *= term2;

            tau *= (idx_t)(e + 1);
            ++i;
        }

        /* Remaining prime factor (if any) is > √m. */
        if (temp > 1) {
            uint64_t p = temp;      /* exponent e = 1 */
            seq_t term  = (seq_t)p + 1;                 /* (p²-1)/(p-1) = p+1 */
            sigma  *= term;

            seq_t term2 = (seq_t)p * (seq_t)p + 1;      /* p² + 1 */
            sigma2 *= term2;

            tau *= 2;   /* e+1 = 2 */
        }

        /* Arithmetic mean condition. */
        bool is_arith = (sigma % tau == 0);

        /* RMS condition. */
        bool is_rms = false;
        if (sigma2 % tau == 0) {
            seq_t t  = sigma2 / tau;
            seq_t sq = floor_sqrt(t);
            if (sq * sq == t) is_rms = true;
        }

        if (!is_arith && !is_rms) {
            ++found;
            if (found == n) return (seq_t)m;
        }
        ++m;
    }
}
