/*
 * A248779  Rectangular array, by antidiagonals: T(m,n) = greatest
 *          (m+1)-th-power-free divisor of n!.
 * https://oeis.org/A248779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) enumerates the rectangular array * T(m,n) = greatest
 *            (m+1)-th power‑free divisor of n!, * by antidiagonals starting
 *            at offset 1 (a(1) = 1)
 * Range:     u8 n <= 29, u16 n <= 81, u32 n <= 195, u64 n <= 473,
 *            u128 n <= 1283, big any n
 * Helpers:   build_primes, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/seq_pow.h"

/* Cached list of primes. */
static idx_t *primes      = NULL;
static size_t prime_cnt   = 0;
static idx_t  prime_limit = 0;

/* Ensure that all primes ≤ limit are available. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (primes)
        free(primes);
    primes      = NULL;
    build_primes(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

/* Convert linear index n (1‑based) to coordinates (m, np)
 * in the antidiagonal enumeration of T(m,n). */
static void
antidiagonal_coords(idx_t n, idx_t *m_out, idx_t *np_out)
{
    idx_t k = 1;
    /* Find smallest k with k(k+1)/2 ≥ n, using 128‑bit product. */
    while ( (__int128)k * (k + 1) / 2 < n )
        ++k;

    idx_t prev_sum = (__int128)(k - 1) * k / 2;   /* terms before this diagonal */
    idx_t m  = n - prev_sum;                     /* row index (1‑based) */
    idx_t np = k - m + 1;                         /* column index (1‑based) */

    *m_out  = m;
    *np_out = np;
}

/* Greatest (m+1)-th power‑free divisor of np!. */
static seq_t
pow_free_divisor(idx_t m, idx_t np)
{
    ensure_primes(np);
    seq_t  result = (seq_t)1;
    idx_t  mod    = m + 1;          /* exponent modulus */

    for (size_t i = 0; i < prime_cnt && primes[i] <= np; ++i) {
        idx_t  p  = primes[i];
        idx_t  t  = np;
        idx_t  vp = 0;

        while (t) {
            t /= p;
            vp += t;
        }

        idx_t exp = vp % mod;
        if (exp == 0)
            continue;

        seq_t pp = seq_pow((seq_t)p, exp);
        result *= pp;
    }
    return result;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* undefined for n < 1 – harmless */

    idx_t m, np;
    antidiagonal_coords(n, &m, &np);
    return pow_free_divisor(m, np);
}
