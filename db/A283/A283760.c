/*
 * A283760  Expansion of (Sum_{i>=1} x^prime(i))*(Sum_{j>=1} x^(j^3)).
 * https://oeis.org/A283760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations n = p + k³, where p is prime
 *            and k ≥ 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, int_cube_root, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Trampoline helpers provided by the build system */
#include "../utils/build_primes.h"    /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */
#include "../utils/is_prime.h"        /* bool is_prime(idx_t x); */
#include "../utils/int_cube_root.h"   /* idx_t int_cube_root(idx_t x); */

/* Static cache so that repeated calls with larger n do not recompute
   the same prime list. */
static idx_t *cached_primes = NULL;
static size_t cached_prime_cnt = 0;
static idx_t cached_prime_limit = 0;

/* Compute a(n) – number of representations n = p + k³. */
seq_t a(idx_t n)
{
    /* Ensure the prime table is at least as large as n */
    if (n > cached_prime_limit) {
        /* Free previous allocation if it exists */
        if (cached_primes != NULL) {
            free(cached_primes);
            cached_primes = NULL;
        }
        build_primes(n, &cached_primes, &cached_prime_cnt);
        cached_prime_limit = n;
    }

    /* Find the largest possible cube root */
    idx_t max_k = int_cube_root(n - 1);

    seq_t count = 0;

    for (idx_t k = 1; k <= max_k; ++k) {
        idx_t cube = k * k * k;      /* k³ ≤ n‑1 by construction */
        idx_t p = n - cube;          /* p ≥ 1 */
        if (p < 2) continue;        /* primes start at 2 */
        if (is_prime(p))
            ++count;
    }
    return count;
}
