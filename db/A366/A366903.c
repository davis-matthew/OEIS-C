/*
 * A366903  The sum of exponentially odious divisors of n.
 * https://oeis.org/A366903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative with a(p^e) = 1 + ∑_{k=1..e, k odd
 *            popcount} p^k
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Test whether k has an odd number of 1‑bits. */
static inline bool is_odious_uint64(uint64_t k)
{
    return __builtin_parityll(k);   /* 1 if popcount(k) is odd */
}

/* Computes a(n) for OEIS A366903.  n ≥ 1. */
seq_t a(idx_t n)
{
    /* Handle n = 0 indirectly – the formula gives 1 anyway. */
    /* Factor n into primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    idx_t temp = n;
    if (temp == 0)        /* n == 0 never occurs in the OEIS range */
        return (seq_t)0;

    /* Factor out 2 */
    if ((temp & 1) == 0) {
        uint64_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        primes[cnt] = 2;
        exps[cnt]   = e;
        ++cnt;
    }

    /* Odd factors, trial division */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }

    /* Remaining prime > sqrt(temp) */
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* Compute the product over prime powers. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];

        seq_t sum  = (seq_t)1;   /* divisor 1 contributes 1 */
        seq_t term = p;          /* p^1 */

        for (idx_t k = 1; k <= e; ++k) {
            if (is_odious_uint64((uint64_t)k))
                sum += term;
            term *= p;           /* advance to p^(k+1) */
        }
        result *= sum;
    }

    return result;
}
