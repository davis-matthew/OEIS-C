/*
 * A372997  a(n) = Sum_{k=1..n} tau( (n/gcd(k,n))^3 ).
 * https://oeis.org/A372997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 419, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   div_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{d|n} phi(d) * tau(d^3)
 *
 * This implementation factorises n, enumerates all divisors using a
 * recursive DFS over the prime factorisation and accumulates
 * phi(d) and tau(d^3) multiplicatively.  No floating point or large
 * intermediate types are required; all arithmetic uses `seq_t`.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/div_rec.h"

/*---------------------------------------------------------------------------*
 *  Helper: factorise unsigned 64‑bit integer
 *---------------------------------------------------------------------------*/
static void factor_uint64(uint64_t n, uint64_t *prm,
                          uint64_t *fld, size_t *cnt)
{
    size_t c = 0;
    if (n % 2 == 0) {
        uint64_t e = 0;
        while (n % 2 == 0) {
            n /= 2;
            ++e;
        }
        prm[c]   = 2;
        fld[c++] = e;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            prm[c]   = p;
            fld[c++] = e;
        }
    }
    if (n > 1) {
        prm[c]   = n;
        fld[c++] = 1;
    }
    *cnt = c;
}

/*---------------------------------------------------------------------------*
 *  Main sequence function
 *---------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }

    uint64_t nn = (uint64_t)n;

    /* factorise n */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t prime_cnt;
    factor_uint64(nn, primes, exps, &prime_cnt);

    /* enumerate divisors and accumulate */
    seq_t sum = (seq_t)0;
    div_rec(primes, exps, prime_cnt, 0, (seq_t)1, (seq_t)1, &sum);

    return sum;
}
