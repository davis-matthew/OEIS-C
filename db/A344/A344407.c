/*
 * A344407  Discriminant of the (2n)-th cyclotomic field Q(zeta_(2n)).
 *          Equivalently, discriminant of the (2n)-th cyclotomic polynomial.
 * https://oeis.org/A344407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 10, i64 n <= 15, i128 n <= 28,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = discriminant of the 2n‑th cyclotomic field Q(zeta_{2n})
 *            = (-1)^{φ(2n)/2} ∏_{p^a∥2n} p^{a·φ(2n) – φ(2n)/(p-1)},
 *            where φ denotes Euler’s totient.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_mul.h"

static seq_t pow_seq(seq_t base, seq_t exp)
{
    seq_t res = (seq_t)1;
    while (exp > 0)
    {
        if (exp & (seq_t)1)
            res = seq_mul(res, base);
        exp >>= 1;
        if (exp)
            base = seq_mul(base, base);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;                  /* disc(2) = 1 */

    uint64_t m = (uint64_t)n * 2ULL;
    uint64_t tmp = m;

    /* factorization of m: primes[·] and exps[·] */
    seq_t primes[16];
    seq_t exps[16];
    size_t cnt = 0;

    if (tmp % 2ULL == 0ULL)          /* factor 2 */
    {
        uint64_t e = 0;
        while (tmp % 2ULL == 0ULL)
        {
            tmp /= 2ULL;
            e++;
        }
        primes[cnt] = 2;
        exps[cnt]   = e;
        cnt++;
    }

    for (uint64_t p = 3ULL; p * p <= tmp; p += 2ULL)   /* odd factors */
    {
        if (tmp % p == 0ULL)
        {
            uint64_t e = 0;
            while (tmp % p == 0ULL)
            {
                tmp /= p;
                e++;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            cnt++;
        }
    }

    if (tmp > 1ULL)                  /* remaining prime */
    {
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        cnt++;
    }

    /* compute φ(m) */
    seq_t phi = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = primes[i];
        seq_t a = exps[i];
        seq_t part = pow_seq(p, a - 1);
        part = seq_mul(part, p - 1);
        phi = seq_mul(phi, part);
    }

    /* sign: negative iff φ(2n) ≡ 2 (mod 4) */
    bool negative = ((phi % (seq_t)4) == (seq_t)2);

    /* compute product ∏ p^{a·φ – φ/(p-1)} */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = primes[i];
        seq_t a = exps[i];

        seq_t term = phi * a - (phi / (p - 1));
        if (term > 0)
        {
            seq_t power = pow_seq(p, term);
            result = seq_mul(result, power);
        }
    }

    if (negative)
        result = -result;

    return result;
}
