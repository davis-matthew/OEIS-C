/*
 * A295442  a(n) = (18*n)!*(5*n)!*(3*n)!/((10*n)!*(9*n)!*(6*n)!*n!).
 * https://oeis.org/A295442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n)!*(5*n)!*(3*n)! / ((10*n)!*(9*n)!*(6*n)!*n!)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   build_primes, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the prime‑exponent form of factorials (Legendre’s formula) and
 * multiply the contributing prime powers.  This keeps all intermediate
 * numbers small enough for seq_t, even on 64‑bit builds where the result
 * itself fits.  The algorithm is O(π(18n) · log n), easily fast enough
 * for n ≤ 10 000. */

#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/seq_pow.h"

static idx_t *prime_list = NULL;
static size_t prime_cnt  = 0;
static idx_t prime_limit = 0;

/* Legendre: exponent of prime p in m! */
static idx_t exp_in_factorial(idx_t m, idx_t p)
{
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Ensure we have primes up to 18*n */
    if (prime_limit < 18 * n) {
        /* Free old list if it existed */
        free(prime_list);
        prime_list = NULL;
        prime_cnt  = 0;
        prime_limit = 18 * n;
        build_primes(prime_limit, &prime_list, &prime_cnt);
    }

    seq_t result = 1;

    /* For each prime p ≤ 18n compute its net exponent in the ratio.  */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > 18 * n) break;

        idx_t e = 0;
        /* Numerators */
        e += exp_in_factorial(18 * n, p);
        e += exp_in_factorial(5  * n, p);
        e += exp_in_factorial(3  * n, p);
        /* Denominators */
        e -= exp_in_factorial(10 * n, p);
        e -= exp_in_factorial(9  * n, p);
        e -= exp_in_factorial(6  * n, p);
        e -= exp_in_factorial(n, p);

        if (e > 0) {
            /* Multiply the prime^e into the result.  */
            result *= seq_pow((seq_t)p, e);
        }
    }

    return result;
}
