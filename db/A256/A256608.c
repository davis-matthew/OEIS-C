/*
 * A256608  Longest eventual period of a^(2^k) mod n for all a.
 * https://oeis.org/A256608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the multiplicative order of 2 modulo the odd part of the
 *            Carmichael * λ(n), i.e. the longest eventual period of a^(2^k)
 *            mod n for all a
 * Range:     u8 n <= 585, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* safety – never requested */

    /* factorize n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute λ(n) */
    seq_t lam = (seq_t)1;                         /* λ(1) = 1 */
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t lam_i;
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == (uint64_t)2)
        {
            if (e == (uint64_t)1)
                lam_i = (seq_t)1;                /* λ(2) = 1 */
            else if (e == (uint64_t)2)
                lam_i = (seq_t)2;                /* λ(4) = 2 */
            else
                lam_i = ((seq_t)1) << (e - 2);   /* λ(2^e) = 2^(e-2),  e>=3 */
        }
        else
        {
            /* λ(p^e) = φ(p^e) = p^(e-1) * (p-1)   (p is an odd prime) */
            lam_i = (seq_t)(p - 1);
            for (uint64_t k = 1; k < e; ++k)
                lam_i *= (seq_t)p;
        }

        /* λ(n) = lcm of the λ(p^e) */
        seq_t g = gcd(lam, lam_i);
        lam = (lam / g) * lam_i;                 /* use lcm = a/gcd(a,b)*b */
    }

    /* odd part of λ(n) */
    seq_t odd = lam;
    while ((odd & (seq_t)1) == (seq_t)0)
        odd >>= 1;

    if (odd == (seq_t)1) return (seq_t)1;       /* period 1 for trivial case */

    /* multiplicative order of 2 modulo odd */
    seq_t pow = (seq_t)2 % odd;
    seq_t k   = (seq_t)1;
    while (pow != (seq_t)1)
    {
        pow = (pow * (seq_t)2) % odd;
        ++k;
    }
    return k;
}
