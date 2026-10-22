/*
 * A347227  Square array T(n,k), n >= 1, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{d|n} mu(d)*mu(n/d)*d^k.
 * https://oeis.org/A347227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 30, i16 n <= 70, i32 n <= 158, i64 n <= 415,
 *            i128 n <= 1138, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(n',k') where T(n,k) = Σ_{d|n} μ(d)*μ(n/d)*d^k
 * interpreting the sequence as the antidiagonal reading of the square
 * array T(n,k) (n ≥ 1, k ≥ 0) with offset 1.
 *
 * This implementation builds μ up to the required n on demand using a
 * simple sieve.  It then computes T(n,k) by enumerating the divisors of n.
 *
 * Complexity: O(√n) per term plus the incremental cost of sieving
 *              μ, which is amortised over all calls.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- lazy Möbius table ---------- */
static int8_t *mu          = NULL;   /* μ[1..] */
static bool   *is_comp    = NULL;   /* is_composite flags */
static uint64_t *primes   = NULL;   /* list of primes */
static size_t primes_cnt  = 0;
static size_t primes_cap  = 0;
static uint64_t mu_limit  = 0;      /* size of allocated μ array */

/* Reallocate the Möbius table up to at least 'limit'.  All arrays are
 * cast from void* to the desired pointer type. */
static void
ensure_mu_upto(uint64_t limit)
{
    if (limit <= mu_limit) return;          /* already done */

    /* expand storage */
    mu      = (int8_t *)realloc(mu,      (limit + 1) * sizeof(int8_t));
    is_comp = (bool *)  realloc(is_comp, (limit + 1) * sizeof(bool));
    if (!mu || !is_comp) { abort(); }

    /* expand prime list storage if necessary */
    if (primes_cap < limit + 32) {
        size_t new_cap = limit + 32;
        primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!primes) abort();
        primes_cap = new_cap;
    }

    /* initialise for n = 1 */
    mu[1]          = 1;
    is_comp[1]     = false;
    primes_cnt     = 0;

    /* linear sieve for μ */
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            mu[i] = -1;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint64_t p = primes[j];
            uint64_t v = i * p;
            if (v > limit) break;
            is_comp[v] = true;
            if (i % p == 0) {
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }
    mu_limit = limit;
}

/* Compute d^k (k ≥ 0) in seq_t; k fits in idx_t */
static seq_t
pow_small(idx_t d, idx_t k)
{
    seq_t r = 1;
    for (idx_t i = 0; i < k; ++i)
        r *= (seq_t)d;
    return r;
}

/* T(n,k) = Σ_{d|n} μ(d) μ(n/d) d^k */
static seq_t
T_value(idx_t n, idx_t k)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;
        idx_t d1 = d;
        idx_t d2 = n / d;

        seq_t mu1 = (seq_t)mu[d1];
        seq_t mu2 = (seq_t)mu[d2];

        sum += mu1 * mu2 * pow_small(d1, k);

        if (d1 != d2)
            sum += mu2 * mu1 * pow_small(d2, k);
    }
    return sum;
}

/* Map an index n (offset 1) to coordinates (n',k') of the antidiagonal
 * of the square array T.  The arrays are 1‑based for n' and 0‑based for k'. */
static void
antidiagonal_coords(idx_t n, idx_t *pn, idx_t *pk)
{
    /* Find s with s*(s+1)/2 ≥ n.  s is the row index in the antidiagonal. */
    idx_t s = (idx_t)((sqrt((long double)(2.0L * n))));  /* rough guess */
    while (s * (s + 1) / 2ULL < n) ++s;
    while (s > 0 && (s - 1) * s / 2ULL >= n) --s;

    idx_t prev = (s - 1) * s / 2ULL;          /* previous diagonal total */
    idx_t off  = n - prev - 1;                /* 0‑based offset on diagonal */

    *pn = off + 1;            /* row (n')  1‑based */
    *pk = s - *pn;            /* column (k') 0‑based */
}

/* --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* safety guard; OEIS offset starts at 1 */

    idx_t nprime, kprime;
    antidiagonal_coords(n, &nprime, &kprime);
    ensure_mu_upto((uint64_t)nprime);
    return T_value(nprime, kprime);
}
