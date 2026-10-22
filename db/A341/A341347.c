/*
 * A341347  a(n) = (1+A003961(A003961(n)))/2 mod 3, where A003961 shifts the
 *          prime factorization of n one step towards larger primes.
 * https://oeis.org/A341347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + A003961(A003961(n))) / 2 (mod 3)
 * Range:     u8 n <= 15, u16 n <= 3923, u32 n <= 9821, u64 n <= 9821,
 *            u128 n <= 9821, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A003961: shift every prime factor of n
   to its next larger prime, keeping exponents unchanged.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

/* Shift prime factorisation of x one step towards larger primes.
   f(x) = ∏ next_prime(p_i)^e_i  for x = ∏ p_i^e_i.          */
static uint64_t shift_u64(uint64_t x)
{
    if (x <= 1)
        return (uint64_t)1;          /* A003961(1) = 1 by definition */

    uint64_t pr[64] = {0};
    uint64_t ex[64] = {0};
    size_t   cnt   = 0;

    factor_u64(x, pr, ex, &cnt);

    uint64_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p_next = next_prime(pr[i]);      /* replace p by next prime */
        for (uint64_t e = 0; e < ex[i]; ++e)      /* keep exponent e */
            res *= p_next;
    }
    return res;
}

/* Main sequence function.                                    */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* outside the offset – undefined behaviour expected */

    /* First shift */
    uint64_t t1 = shift_u64((uint64_t)n);

    /* Second shift */
    uint64_t t2 = shift_u64(t1);

    /* Compute ((1 + t2) / 2) mod 3 using seq_t arithmetic. */
    seq_t val = (seq_t)t2;
    seq_t r   = (val + (seq_t)1) / (seq_t)2;
    return r % (seq_t)3;
}
