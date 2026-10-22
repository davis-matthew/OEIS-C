/*
 * A330749  a(n) = gcd(n, A064989(n)), where A064989 is fully multiplicative
 *          with a(2) = 1 and a(prime(k)) = prime(k-1) for odd primes.
 * https://oeis.org/A330749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A064989(n)) A064989 is fully multiplicative, a(2)
 *            = 1, a(p) = prev_prime(p) for odd primes p
 * Range:     u8 n <= 1694, u16 n <= 8081, u32 n <= 8081, u64 n <= 8081,
 *            u128 n <= 8081, big any n
 * Helpers:   factor_u64, gcd, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/prev_prime.h"

/* Fast exponentiation for seq_t */
static seq_t pow_seq_t(seq_t base, int exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* sequence is defined for n >= 1 */

    /* Factor n: obtain prime factors and exponents. */
    /* Arrays sized to hold enough factors for 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e = (int)exps[i];

        if (p == 2)        /* a(2) = 1 – nothing to multiply */
            continue;

        uint64_t prev = prev_prime(p);
        /* Multiply product by prev^e */
        prod *= pow_seq_t((seq_t)prev, e);
    }

    return gcd((seq_t)n, prod);
}
