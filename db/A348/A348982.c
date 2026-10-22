/*
 * A348982  a(n) = Sum_{d|n} psi(n/d) * A322582(d), where psi is Dedekind psi
 *          (A001615), A322582(n) = n - A003958(n), and A003958 is fully
 *          multiplicative with a(p) = (p-1).
 * https://oeis.org/A348982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} psi(n/d) * ( d - product_{p|d} (p-1)^{exp(d,p)}
 *            )
 * Range:     u8 n <= 33, u16 n <= 1439, u32 n <= 9434, u64 n <= 9434,
 *            u128 n <= 9434, big any n
 * Helpers:   _rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where psi(m) = Dedekind psi = m * prod_{p|m} (1+1/p) */
/* This implements the Dirichlet convolution of A001615 and A322582. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_rec.h"

/* Forward declaration of the function to be implemented. */
seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Factorise n */
    uint64_t primes[64];
    size_t exps[64];
    size_t pcnt = 0;

    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            size_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (m > 1) {
        primes[pcnt] = m;
        exps[pcnt] = 1;
        ++pcnt;
    }

    size_t cho_exp[64];   /* storage for current exponent choices, 64 > max possible primes for 64‑bit n */
    seq_t sum = 0;

    _rec(n, primes, exps, pcnt, 0, cho_exp, (seq_t)1, (seq_t)1, &sum);

    return sum;
}
