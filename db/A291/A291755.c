/*
 * A291755  Compound filter (multiplicative order of 2 mod 2n+1 &
 *          eulerphi(2n+1)): a(n) = P(A002326(n), A037225(n)), where P(n,k) is
 *          sequence A000027 used as a pairing function.
 * https://oeis.org/A291755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(2 + ((A002326(n) + A000010(2n+1))^2) - A002326(n)
 *            - 3*A000010(2n+1)) where A002326(n) is the multiplicative order
 *            of 2 modulo (2n+1), A000010 is Euler's totient
 * Range:     u8 n <= 4, u16 n <= 97, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* local helper: modular exponentiation (base^exp mod mod) */
static seq_t mod_pow_2(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp != 0) {
        if (exp & 1ULL)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* local helper: compute Euler's totient of n (n is small enough to fit in 64 bits) */
static seq_t phi_of(seq_t n)
{
    seq_t result = n;
    seq_t temp   = n;
    /* trial division up to sqrt(temp) */
    for (uint64_t p = 2; (uint64_t)(p * p) <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0)
                temp /= p;
            result -= result / (seq_t)p;
        }
    }
    if (temp > 1)
        result -= result / temp;
    return result;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* m = 2n + 1 */
    seq_t m = ((seq_t)n) * 2 + 1;

    /* φ(m) */
    seq_t phi = phi_of(m);

    /* order of 2 modulo m */
    seq_t order = phi;

    /* factorisation of phi (phi fits in 64‑bit for the intended range) */
    uint64_t phi_val = (uint64_t)phi;
    uint64_t factor[64];
    size_t   fcnt   = 0;
    uint64_t rem = phi_val;
    for (uint64_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            factor[fcnt++] = p;
            while (rem % p == 0)
                rem /= p;
        }
    }
    if (rem > 1)
        factor[fcnt++] = rem;

    /* minimise the order by dividing out factors as long as 2^d ≡ 1 (mod m) */
    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t p = factor[i];
        while (order % (seq_t)p == 0) {
            seq_t cand = order / (seq_t)p;
            uint64_t exp = (uint64_t)cand; /* phi is small, fits 64‑bit */
            if (mod_pow_2(2, exp, m) == 1)
                order = cand;
            else
                break;
        }
    }

    /* compute a(n) using the explicit formula */
    seq_t sum = order + phi;
    seq_t temp = sum * sum;
    temp -= order;
    temp -= 3 * phi;
    temp += 2;
    return temp / 2;
}
