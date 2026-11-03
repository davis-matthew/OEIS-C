/*
 * A378663  Number of divisors d of n such that sigma(d) < 2*d < A003961(d),
 *          where A003961 is fully multiplicative with a(p) = nextprime(p).
 * https://oeis.org/A378663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [ sigma(d) < 2*d < A003961(d) ] , where A003961
 *            is multiplicative with a(p)=next_prime(p)
 * Range:     u8 n <= 62, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9400 (2026-11-01)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

/* Recursive divisor enumeration.  cur_d, cur_sigma, cur_a are maintained
   multiplicatively as the product over the primes processed so far. */
static void count_valid_divisors(size_t depth,
                                 const uint64_t *pr,
                                 const uint64_t *np,   /* next prime of pr[i] */
                                 const int *ex,
                                 size_t num,
                                 seq_t cur_sigma,
                                 seq_t cur_a,
                                 seq_t *cnt)
{
    if (depth == num) {
        seq_t two_d = (seq_t)1ULL << 1; /* 2*1 for d=1, handled by cur_d implicitly as product equal 1 */
        /* The product cur_sigma already equals sigma(d).
           cur_a equals A003961(d).
           cur_d itself is not needed beyond the inequality. */
        /* We can reconstruct 2*d: d is the product of p^k for each prime.
           But we can carry d as seq_t too, but it is not used in inequality except for 2*d.
           Since cur_sigma and cur_a are sufficient, we compute d from the
           multiplicative recursion: we keep d in recursion as seq_t argument. */
        /* To avoid carrying d separately, we can compute 2*d by multiplying
           cur_d (passed as part of recursion).  We add an extra parameter for d. */
        return;
    }
}

static uint64_t next_prime_uint64(uint64_t p)
{
    for (uint64_t q = p + 1;; ++q)
        if (is_prime(q))
            return q;
}

/* Helper that inlines the divisor enumeration without separate
   count_valid_divisors (uses the same logic but carries the divisor). */
static void enumerate_divisors(size_t depth,
                               const uint64_t *pr,
                               const uint64_t *np,
                               const int *ex,
                               size_t num,
                               seq_t cur_d,
                               seq_t cur_sigma,
                               seq_t cur_a,
                               seq_t *cnt)
{
    if (depth == num) {
        seq_t two_d = cur_d * 2;
        if (cur_sigma < two_d && two_d < cur_a)
            (*cnt)++;
        return;
    }
    uint64_t p = pr[depth];
    uint64_t np_p = np[depth];
    uint64_t p_power = 1;          /* p^k  (uint64_t) */
    seq_t sigma_factor = 1;        /* sigma(p^k) as seq_t */
    seq_t a_factor     = 1;        /* np_p^k  (seq_t) */
    for (int k = 0; k <= ex[depth]; ++k) {
        /* Recurse with current exponent choice */
        enumerate_divisors(depth + 1,
                           pr,
                           np,
                           ex,
                           num,
                           cur_d * p_power,
                           cur_sigma * sigma_factor,
                           cur_a * a_factor,
                           cnt);

        /* Prepare k+1 */
        if (k != ex[depth]) {
            p_power *= p;
            sigma_factor += (seq_t)p_power;     /* sigma(p^k) = sigma(p^(k-1)) + p^k */
            a_factor *= (seq_t)np_p;            /* np_p^k = np_p^(k-1) * np_p */
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* safety; offset is 1 */

    /* Prime factorisation of n. */
    uint64_t primes[64];
    uint64_t exps[64];    /* uint64_t for exponents (will cast to int) */
    size_t num_primes = 0;
    factor_u64(n, primes, exps, &num_primes);

    /* Compute next prime for each distinct prime factor. */
    uint64_t nextp[64];
    for (size_t i = 0; i < num_primes; ++i)
        nextp[i] = next_prime_uint64(primes[i]);

    /* Convert exps to int array required by recursion (values fit in int). */
    int exps_int[64];
    for (size_t i = 0; i < num_primes; ++i)
        exps_int[i] = (int)exps[i];

    seq_t count = 0;
    enumerate_divisors(0,
                       primes,
                       nextp,
                       exps_int,
                       num_primes,
                       (seq_t)1,   /* cur_d starts at 1 */
                       (seq_t)1,   /* sigma(1) = 1 */
                       (seq_t)1,   /* A003961(1) = 1 */
                       &count);
    return count;
}
