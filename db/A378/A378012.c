/*
 * A378012  a(n) = b(10*n+1), with the sequence {b(n)} having Dirichlet g.f.
 *          Product_{chi} L(chi,s), where chi runs through all Dirichlet
 *          characters modulo 10; 10th column of A378007.
 * https://oeis.org/A378012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(10*n + 1) where b is multiplicative: * b(2^e) = b(5^e)
 *            = 0 * if p ≡ 1 (mod 10): b(p^e) = C(e+3, 3) = (e+3)(e+2)(e+1)/6
 *            * if p ≡ 9 (mod 10): b(p^e) = 0 if e odd; e/2 + 1 if e even * if
 *            p ≡ 3 or 7 (mod 10): b(p^e) = 1 if 4 | e; 0 otherwise *
 *            otherwise b(p^e) = 0 * (b(1)=1 by definition)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* local per‑prime power value */
static seq_t
prime_power_value(uint64_t p, idx_t e)
{
    switch (p % 10) {
    case 1: {
        seq_t E = (seq_t)e;
        /* C(e+3,3) = (e+3)(e+2)(e+1) / 6 */
        return (E + (seq_t)3) * (E + (seq_t)2) * (E + (seq_t)1) / (seq_t)6;
    }
    case 9:
        if (e & 1)
            return (seq_t)0;
        return (seq_t)(e / 2 + 1);
    case 3:
    case 7:
        return ((e & 3) == 0) ? (seq_t)1 : (seq_t)0;
    default:
        return (seq_t)0;
    }
}

seq_t
a(idx_t n)
{
    /* The argument to b is 10*n + 1, which is always odd and never divisible by 5. */
    uint64_t m = (uint64_t)10 * n + 1ULL;

    /* Trial division factorisation – fast enough for m ≤ 10^6 (n ≤ 10^5). */
    uint64_t primes[64];
    idx_t     exps[64];
    size_t    nf = 0;

    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            exps[nf] = 0;
            while (t % p == 0) {
                t /= p;
                ++exps[nf];
            }
            primes[nf] = p;
            ++nf;
        }
    }
    if (t > 1) {
        primes[nf] = t;
        exps[nf]   = 1;
        ++nf;
    }

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < nf; ++i) {
        seq_t val = prime_power_value(primes[i], exps[i]);
        if (val == (seq_t)0)
            return (seq_t)0;
        res *= val;
    }
    return res;
}
