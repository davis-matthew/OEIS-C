/*
 * A344877  a(n) = gcd(n, A344875(n)), where A344875 is multiplicative with
 *          a(2^e) = 2^(1+e) - 1, and a(p^e) = p^e -1 for odd primes p.
 * https://oeis.org/A344877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1439, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = gcd(n, A344875(n))
 * where A344875 is multiplicative with A344875(2^e) = 2^(e+1) – 1
 * and A344875(p^e) = p^e – 1 for odd primes p
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* sequence offset starts at 1, but we guard against n == 0 */
    if (n == 0) return 0;

    /* Convert n to a native type for manipulation */
    uint64_t nn = (uint64_t)n;

    /* Allocate arrays for prime factors and exponents (enough for 64‑bit n) */
    static const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;

    /* Factor n */
    factor_u64(nn, primes, exps, &cnt);

    /* Compute the multiplicative function A344875(n) */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term;
        if (p == 2) {
            term = pow_uint(2, e + 1);
            term -= (seq_t)1;
        } else {
            term = pow_uint(p, e);
            term -= (seq_t)1;
        }
        prod *= term;
    }

    /* Return gcd(n, A344875(n)) */
    return gcd((seq_t)nn, prod);
}
