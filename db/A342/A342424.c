/*
 * A342424  a(n) = Sum_{k=1..n} gcd(k,n)^(n/gcd(k,n)).
 * https://oeis.org/A342424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * d^(n/d)
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 53, u64 n <= 113,
 *            u128 n <= 233, big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, but safe */

    /* --- factorisation of n --- */
    uint64_t primes[64];
    int      exps[64];
    size_t   pcnt = 0;

    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* --- generate all divisors of n --- */
    seq_t divs[65536];          /* ample space for <= 65536 divisors */
    size_t dcnt = 1;            /* current count of divisors */
    divs[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p  = primes[i];
        int      e  = exps[i];
        size_t   old = dcnt;
        seq_t    p_pow = 1;
        for (int k = 1; k <= e; ++k) {
            p_pow *= (seq_t)p;          /* raise prime to k */
            for (size_t j = 0; j < old; ++j)
                divs[dcnt++] = divs[j] * p_pow;
        }
    }

    /* --- compute the sum over divisors --- */
    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        seq_t d    = divs[i];
        idx_t nd   = (idx_t)(( (seq_t)n ) / d);   /* n/d, fits into idx_t */
        seq_t phi_val = phi(nd);                  /* φ(n/d) */
        seq_t pow_val = big_pow(d, (seq_t)nd);    /* d^(n/d) */
        sum += phi_val * pow_val;
    }

    return sum;
}
