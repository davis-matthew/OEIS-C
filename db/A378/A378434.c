/*
 * A378434  Arithmetic mean between the Dirichlet inverses of {sum of unitary
 *          divisors} and {sum of squarefree divisors}.
 * https://oeis.org/A378434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 59, i16 n <= 8992, i32 n <= 8992, i64 n <= 8992,
 *            i128 n <= 8992, big any n
 * Helpers:   g_squarefree_inv_prime_power, g_unitary_inv_prime_power
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
#include "../utils/g_squarefree_inv_prime_power.h"
#include "../utils/g_unitary_inv_prime_power.h"

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
