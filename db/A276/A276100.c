/*
 * A276100  a(n) = (15*n)!*(n/2)!/((15*n/2)!*(5*n)!*(3*n)!).
 * https://oeis.org/A276100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15n)! * (n/2)! / ( (15n/2)! * (5n)! * (3n)! )
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses prime factorisation of factorials. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;
static size_t prime_count  = 0;
static idx_t prime_limit   = 0;

/* Ensure prime list up to 'limit' is constructed. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (prime_list)
        free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Exponent of prime 'p' in m! (Legendre's formula). */
static idx_t
exp_fact(idx_t m, idx_t p)
{
    idx_t e = 0;
    idx_t pp = p;
    while (pp <= m) {
        e += m / pp;
        if (pp > m / p)
            break;
        pp *= p;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    bool is_even = (n & 1) == 0;
    idx_t k = n >> 1;          /* floor(n/2) */

    if (is_even) {
        /* a(2k) = (30k)! * k! / ( (15k)! * (10k)! * (6k)! )  */
        idx_t limit = 30 * k;
        ensure_primes(limit);

        seq_t result = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_list[i];
            if (p > limit)
                break;
            idx_t e = exp_fact(30 * k, p) + exp_fact(k, p)
                    - exp_fact(15 * k, p) - exp_fact(10 * k, p)
                    - exp_fact(6  * k, p);
            if (e == 0)
                continue;
            seq_t mult = seq_pow((seq_t)p, e);
            result = seq_mul(result, mult);
        }
        return result;
    } else {
        /* a(2k+1) = (30k+15)! * (2k+2)! * (15k+8)! * 4^(14k+7)
                    -----------------------------------------------
                    (30k+16)! * (k+1)! * (10k+5)! * (6k+3)!                */
        idx_t limit = 30 * k + 16;
        ensure_primes(limit);

        seq_t result = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_list[i];
            if (p > limit)
                break;
            idx_t e = 0;
            e += exp_fact(30 * k + 15, p);
            e += exp_fact(2 * k + 2, p);
            e += exp_fact(15 * k + 8, p);
            if (p == 2)
                e += 2 * (14 * k + 7);          /* 4^(14k+7) = 2^(2(14k+7)) */
            e -= exp_fact(30 * k + 16, p);
            e -= exp_fact(k + 1, p);
            e -= exp_fact(10 * k + 5, p);
            e -= exp_fact(6 * k + 3, p);
            if (e == 0)
                continue;
            seq_t mult = seq_pow((seq_t)p, e);
            result = seq_mul(result, mult);
        }
        return result;
    }
}
