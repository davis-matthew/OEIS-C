/*
 * A400203  Triangle read by rows: T(n,d) = Sum_{x=1..floor(n/d), gcd(x,d)=1}
 *          mu(x) where mu(x) is the Möbius function.
 * https://oeis.org/A400203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, new
 * Formula:   a(n) = T(m,d) where m = ceil((sqrt(8*n+1)-1)/2), d = n -
 *            m*(m-1)/2, and T(m,d) = Σ_{x=1..⌊m/d⌋, gcd(x,d)=1} μ(x). This
 *            implements the definition of OEIS A400203
 * Range:     i8 n <= 9289, i16 n <= 9289, i32 n <= 9289, i64 n <= 9289,
 *            i128 n <= 9289, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* GCD on unsigned 64‑bit indices */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Möbius function cache (int8_t already holds –1,0,1) */
static int8_t *mu_cache = NULL;   /* indices 0..mu_size; mu_cache[0] unused */
static idx_t mu_size   = 0;

/* Linear sieve for μ[1..limit] */
static void recompute_mu(idx_t limit)
{
    int8_t *newmu = (int8_t *)malloc((limit + 1) * sizeof(int8_t));
    if (!newmu) abort();

    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    idx_t *primes   = (idx_t *)malloc(limit * sizeof(idx_t));
    if (!is_comp || !primes) abort();

    size_t pc = 0;
    newmu[1] = 1;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[pc++] = i;
            newmu[i] = -1;
        }
        for (size_t j = 0; j < pc; ++j) {
            idx_t p = primes[j];
            idx_t v = i * p;
            if (v > limit) break;
            is_comp[v] = true;
            if (i % p == 0) {
                newmu[v] = 0;
                break;
            } else {
                newmu[v] = -newmu[i];
            }
        }
    }

    free(mu_cache);
    free(is_comp);
    free(primes);

    mu_cache = newmu;
    mu_size  = limit;
}

static void ensure_mu(idx_t limit)
{
    if (limit <= mu_size) return;
    recompute_mu(limit);
}

/* Find the row m such that n lies in the m‑th row of the triangle
   (the m‑th row contains m elements). */
static idx_t find_row(idx_t n)
{
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        __int128 s = (__int128)mid * (mid + 1) / 2;
        if (s >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Convert flattened index n to (m,d) */
    idx_t m = find_row(n);
    __int128 prev = (__int128)(m - 1) * m / 2;
    idx_t d = n - (idx_t)prev;          /* 1 <= d <= m */

    /* Compute T(m,d) = Σ μ(x) over x <= floor(m/d) with gcd(x,d)=1 */
    idx_t limit = m / d;
    ensure_mu(limit);

    seq_t sum = 0;
    for (idx_t x = 1; x <= limit; ++x) {
        if (gcd_idx(x, d) == 1) {
            sum += (seq_t)mu_cache[(size_t)x];
        }
    }
    return sum;
}
