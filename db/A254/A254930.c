/*
 * A254930  Fundamental positive solution x = x2(n) of the second class of the
 *          Pell equation x^2 - 2*y^2 = A001132(n), n >= 1 (primes congruent
 *          to 1 or 7 mod 8).
 * https://oeis.org/A254930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_i32, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the second class fundamental solution x of the Pell
   equation x^2 - 2*y^2 = p, where p is the n-th prime ≡ 1 or 7 (mod 8).
   The result is obtained by brute‑forcing y until two solutions are found.
   (OEIS entry A254930) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"
#include "../utils/next_prime.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Upper bound on the number of required primes (≈10 000th such prime). */
#define MAX_PRIMES 20000

static uint64_t prime_list[MAX_PRIMES];
static size_t   prime_count = 0;
static bool     primes_inited = false;

/* Generate enough primes ≡ 1 or 7 (mod 8) to answer any requested n. */
static void init_primes(void)
{
    if (primes_inited)
        return;

    /* Estimate an upper bound sufficient for the first MAX_PRIMES such primes.
       The _prime_limit_for_idx helper returns a crude bound for the n‑th prime. */
    idx_t limit = _prime_limit_for_idx(MAX_PRIMES + 100);
    uint64_t *tmp;
    size_t tmpcnt;

    build_primes_upto(limit, &tmp, &tmpcnt);

    for (size_t i = 0; i < tmpcnt && prime_count < MAX_PRIMES; ++i) {
        uint64_t p = tmp[i];
        if ((p & 7ULL) == 1ULL || (p & 7ULL) == 7ULL)
            prime_list[prime_count++] = p;
    }

    primes_inited = true;
}

/* ---------------------------------------------------------------------- */
/*  Sequence function --------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined below offset */

    init_primes();

    if ((idx_t)prime_count < n)          /* safety check for very large n */
        return 0;

    /* n-th prime ≡ 1 or 7 (mod 8) */
    uint64_t p = prime_list[n - 1];

    /* Search y = 1, 2, … for solutions of x² – 2y² = p.
       Stop when the second solution is found. */
    seq_t first = 0, second = 0;
    for (uint64_t yy = 1;; ++yy) {
        seq_t y = (seq_t)yy;
        seq_t val = (seq_t)p + (seq_t)2 * y * y;
        seq_t x = floor_sqrt(val);
        if (x * x == val) {
            if (!first)
                first = x;
            else {
                second = x;
                break;
            }
        }
    }
    return second;
}
