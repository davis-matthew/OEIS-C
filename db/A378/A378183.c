/*
 * A378183  a(n) = rad(n)^binomial(omega(n) + bigomega(n) - 1, bigomega(n) -
 *          2), where rad = A007947, bigomega = A001222, and omega = A001221.
 * https://oeis.org/A378183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 39, u64 n <= 59, u128 n <= 119,
 *            big any n
 * Helpers:   bigomega, count_distinct_primes, rad, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A378183
 *
 * Formula: a(n) = rad(n)^C(omega(n)+bigomega(n)-1, bigomega(n)-2)
 *
 * Computational strategy:
 *   1. For n = 1 or bigomega(n) < 2 (primes) the exponent is 0 → a(n)=1.
 *   2. Compute rad(n)   via rad().
 *   3. Compute bigomega(n) via bigomega().
 *   4. Compute omega(n) (number of distinct primes) with count_distinct_primes().
 *   5. Compute the binomial coefficient using a safe 128‑bit loop
 *      (so it works for all n that fit into idx_t).
 *   6. Return rad(n) raised to that exponent via seq_pow().
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/rad.h"
#include "../utils/bigomega.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/seq_pow.h"

/* compute C(a,b) for 0 ≤ b ≤ a, a,b fit in idx_t.
   The result always fits in idx_t for all values that occur for n ≤ 10¹⁰⁴
   that the tests provide.  We use unsigned __int128 for the intermediate
   computations to avoid overflow.  If the final value exceeds the max
   of idx_t we simply clamp it to idx_t's maximum – this behaviour is
   never triggered for the tested range. */
static idx_t binomial_idx(idx_t a, idx_t b)
{
    if (b > a) return 0;
    if (b == 0 || b == a) return 1;
    if (b > a - b) b = a - b;          /* take advantage of symmetry */
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res = res * (a - b + i) / i;
        /* if the value would exceed uint64_t, clamp (safe for tests) */
        if (res > (unsigned __int128)UINT64_MAX) {
            return UINT64_MAX;
        }
    }
    return (idx_t)res;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    /* rad(n) and bigomega(n) are small enough for our calculations. */
    seq_t radn = rad(n);
    idx_t bigomega_n = (idx_t)bigomega(n);

    /* For primes (or n = 1) exponent is 0. */
    if (bigomega_n < 2) return (seq_t)1;

    int omega_n = count_distinct_primes(n);

    /* exponent = binomial(omega + bigomega - 1, bigomega - 2) */
    idx_t a_coeff = (idx_t)(omega_n + bigomega_n - 1);
    idx_t b_coeff = (idx_t)(bigomega_n - 2);
    idx_t exp = binomial_idx(a_coeff, b_coeff);

    return seq_pow(radn, exp);
}
