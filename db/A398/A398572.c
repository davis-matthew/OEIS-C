/*
 * A398572  a(n) = A001222(A000026(n)).
 * https://oeis.org/A398572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = Σ_{p^e‖n} (1 + Ω(e)) (A001222(A000026(n)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is additive: a(mn)=a(m)+a(n) if gcd(m,n)=1. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return 0;                     /* a(1) = 0 */
    }

    uint64_t N = (uint64_t)n;
    /* maximal number of distinct prime factors for a 64‑bit integer is < 16,
       so arrays of size 64 are more than enough. */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt = 0;

    /* Factor N into primes^exponents.  The helper writes the number of
       distinct factors into *cnt. */
    factor_u64(N, primes, exponents, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        /* Ω(e) : number of prime factors of the exponent, counted with multiplicity */
        seq_t omega_e = bigomega(exponents[i]);
        sum += 1 + omega_e;           /* add 1 + Ω(e) for this prime power */
    }
    return sum;
}
