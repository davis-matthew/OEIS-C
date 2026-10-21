/*
 * A241917  If n is a prime with index i, p_i, a(n) = i, (with a(1)=0),
 *          otherwise difference (i-j) of the indices of the two largest
 *          primes p_i, p_j, i >= j in the prime factorization of n: a(n) =
 *          A061395(n) - A061395(A052126(n)).
 * https://oeis.org/A241917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A061395(n) - A061395(A052126(n)) with special case a(1) =
 *            0 and a(p) = index of prime p
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/prime_pi.h"
#include "../utils/factor_u64.h"

/* Return the OEIS A241917 sequence value for index n. */
seq_t a(idx_t n)
{
    if (n <= 1)  /* a(1) = 0 */
        return 0;

    /* Prime case: a(p) = index of the prime p */
    if (is_prime(n))
        return (seq_t)prime_pi(n);

    /* Composite case */
    /* Factor n into distinct primes (pr) and exponents (ex). */
    uint64_t primes_f[64];
    uint64_t exps_f[64];
    size_t cnt = 0;
    factor_u64(n, primes_f, exps_f, &cnt);

    /* Track the largest and second largest prime indices, with multiplicity. */
    idx_t max_idx   = 0;   /* index of largest prime factor */
    size_t max_exp  = 0;   /* total multiplicity of that prime in n */
    idx_t second_idx = 0;  /* index of second largest prime factor */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes_f[i];
        size_t   exp = exps_f[i];
        idx_t    idx = (idx_t)prime_pi(p);   /* index of the prime factor */

        if (idx > max_idx) {
            second_idx = max_idx;
            max_idx    = idx;
            max_exp    = exp;
        } else if (idx == max_idx) {
            max_exp += exp;
        } else if (idx > second_idx) {
            second_idx = idx;
        }
    }

    /* If the largest prime occurs at least twice, the two largest primes are equal. */
    if (max_exp >= 2)
        return 0;

    /* If there is only one distinct prime factor (should not happen here, but safe). */
    if (second_idx == 0)
        return 0;

    /* Return the difference of indices. */
    return (seq_t)(max_idx - second_idx);
}
