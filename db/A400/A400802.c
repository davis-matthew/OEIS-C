/*
 * A400802  Number of orbits of exponents d, 1 <= d <= 2^n-2, under
 *          multiplication by 2 modulo 2^n-1, such that the power map x -> x^d
 *          on GF(2^n) is almost perfect nonlinear (APN).
 * https://oeis.org/A400802
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more, new
 * Formula:   a(n) = φ(n) - 1 + [5|n] for even n (n ≥ 2) a(n) = 2φ(n) + 3 +
 *            2·[5|n] for odd n ≥ 11 (odd n = 3, 5, 7, 9 are special: a(3)=1,
 *            a(5)=5, a(7)=11, a(9)=13)
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   a_special_odd, build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Utility functions from the OEIS utils library */
#include "../utils/build_primes_upto.h"
#include "../utils/int_sqrt.h"
#include "../utils/a_special_odd.h"

/* Static cache of primes up to the largest requested limit */
static uint64_t *prime_list = NULL;
static size_t    prime_count = 0;
static idx_t     prime_limit  = 0;

/* Ensure that primes up to 'limit' are available */
static void
ensure_primes(idx_t limit)
{
    if (prime_list != NULL && prime_limit >= limit)
        return;

    free(prime_list);
    prime_list  = NULL;
    prime_count = 0;
    prime_limit = 0;

    if (limit > 0) {
        build_primes_upto(limit, &prime_list, &prime_count);
        prime_limit = limit;
    }
}

/* Compute Euler's totient φ(n), where n fits into idx_t (≤ 1e4). */
static seq_t
phi_of(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t result = (seq_t)n;
    uint64_t tmp = (uint64_t)n;

    /* Obtain primes up to √n */
    idx_t sqrt_n = int_sqrt(n);
    ensure_primes(sqrt_n);

    /* Trial division */
    for (size_t i = 0; i < prime_count && prime_list[i] <= sqrt_n; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            while (tmp % p == 0)
                tmp /= p;
            result -= result / (seq_t)p;
        }
    }

    /* Remaining prime factor > √n */
    if (tmp > 1)
        result -= result / (seq_t)tmp;

    return result;
}

/* OEIS sequence function */
seq_t
a(idx_t n)
{
    if (n < 2)
        return 0;   /* undefined for n < 2, but a sane default */

    /* Special small odd indices */
    if ((n & 1ULL) && n < 11)
        return a_special_odd(n);

    seq_t phi = phi_of(n);
    seq_t add = (n % 5 == 0) ? 1 : 0;

    if ((n & 1ULL) == 0) {          /* even n */
        return phi - 1 + add;
    } else {                        /* odd n ≥ 11 */
        return 2 * phi + 3 + 2 * add;
    }
}
