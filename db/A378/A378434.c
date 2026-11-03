/*
 * A378434  Arithmetic mean between the Dirichlet inverses of {sum of unitary
 *          divisors} and {sum of squarefree divisors}.
 * https://oeis.org/A378434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 59, i16 n <= 12209, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 88 terms through n = 8992 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (A158523(n)+A178450(n))/2
 *
 * A158523 is the Dirichlet inverse of the unitary-sum σ^*(n)
 * A178450 is the Dirichlet inverse of the square‑free sum σ_sf(n)
 * The function below computes the arithmetic mean of those inverses.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Dirichlet inverse of σ_sf(p^k) where σ_sf(p^k)=1+p for k≥1 (square‑free sum) */
static seq_t g_squarefree_inv_prime_power(seq_t p, idx_t e)
{
    seq_t f[32];
    seq_t g[32];

    f[0] = 1;   /* σ_sf(1) = 1 */
    g[0] = 1;   /* g(1) = 1 */

    seq_t fp1 = 1 + p;                /* σ_sf(p^k) for k≥1 */
    for (idx_t k = 1; k <= e; ++k) {
        f[k] = fp1;
    }

    for (idx_t k = 1; k <= e; ++k) {
        seq_t sum = f[k];
        for (idx_t i = 1; i < k; ++i) {
            sum += g[i] * f[k - i];
        }
        g[k] = -sum;
    }
    return g[e];
}

/* Dirichlet inverse of σ^*(p^k) where σ^*(p^k)=1+p^k (unitary sum) */
static seq_t g_unitary_inv_prime_power(seq_t p, idx_t e)
{
    /* Arrays hold values for 0..e (e <= 30 for n <= 10000) */
    seq_t pow_p[32];
    seq_t f[32];
    seq_t g[32];

    pow_p[0] = 1;
    f[0]    = 1;   /* σ^*(1) = 1 */
    g[0]    = 1;   /* g(1) = 1 */

    for (idx_t k = 1; k <= e; ++k) {
        pow_p[k] = pow_p[k - 1] * (seq_t)p;
        f[k]     = pow_p[k] + 1;      /* 1 + p^k */
    }

    for (idx_t k = 1; k <= e; ++k) {
        seq_t sum = f[k];
        for (idx_t i = 1; i < k; ++i) {
            sum += g[i] * f[k - i];
        }
        g[k] = -sum;
    }
    return g[e];
}

/* Simple trial division factorisation into primes and exponents. */
static void factorize_uint64(idx_t n, uint64_t primes[], idx_t exps[], size_t *cnt)
{
    uint64_t nn = (uint64_t)n;
    *cnt = 0;
    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            size_t e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            primes[*cnt]   = p;
            exps[*cnt]     = (idx_t)e;
            ++(*cnt);
        }
    }
    if (nn > 1) {
        primes[*cnt]   = nn;
        exps[*cnt]     = 1;
        ++(*cnt);
    }
}

/* Compute a(n) as required by the OEIS sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* n=0 is outside the defined range */

    uint64_t primes[32];
    idx_t exps[32];
    size_t cnt;
    factorize_uint64(n, primes, exps, &cnt);

    seq_t gU = 1;   /* Dirichlet inverse of σ^* */
    seq_t gS = 1;   /* Dirichlet inverse of σ_sf */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = exps[i];
        gU *= g_unitary_inv_prime_power(p, e);
        gS *= g_squarefree_inv_prime_power(p, e);
    }

    return (gU + gS) / 2;
}
