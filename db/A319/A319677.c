/*
 * A319677  Denominator of A047994(n)/n where A047994 is the unitary totient
 *          function.
 * https://oeis.org/A319677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 87 terms through n = 9295 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A319677 : Denominator of A047994(n)/n  
 *  a(n) = n / gcd(n, ∏_{p^e||n} (p^e - 1))
 *  where the product is over the distinct prime powers of n.
 *
 *  Implementation uses the provided seq_mul and seq_gcd functions.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static seq_t unitaryTotientMinusOneProduct(uint64_t n)
{
    /* compute P = ∏ (p^e - 1) for the factorisation of n */
    seq_t prod = 1;
    uint64_t m = n;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t pe = 1;
            while (m % p == 0) {
                pe *= p;
                m /= p;
            }
            seq_t term = (seq_t)pe - 1;
            prod = seq_mul(prod, term);
        }
    }
    if (m > 1) {                     /* remaining prime factor */
        seq_t term = (seq_t)m - 1;
        prod = seq_mul(prod, term);
    }
    return prod;
}

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1 only */
    uint64_t val = (uint64_t)n;                /* cast n to native integer for factoring */
    seq_t prod = unitaryTotientMinusOneProduct(val);

    seq_t g = gcd((seq_t)val, prod);
    return ((seq_t)val) / g;
}
