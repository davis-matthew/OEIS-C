/*
 * A384040  The number of integers k from 1 to n such that gcd(n,k) is a
 *          cubefull number.
 * https://oeis.org/A384040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(p^e) = (p^3-p^2+1)*p^(e-3) for e>=3, p*(p-1) for e=2, p-1
 *            otherwise (multiplicative function)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow_seq_uint(seq_t base, int exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

static inline seq_t local_factor(uint64_t p, int e)
{
    seq_t pseq = (seq_t)p;
    if (e == 1)
        return pseq - 1;
    else if (e == 2)
        return pseq * (pseq - 1);
    else {
        seq_t p2 = pseq * pseq;
        seq_t p3 = p2 * pseq;
        seq_t term = p3 - p2 + 1;           /* p^3 - p^2 + 1 */
        term *= pow_seq_uint(pseq, e - 3);   /* multiply by p^(e-3) */
        return term;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)      /* not used, but safe */
        return 0;
    seq_t result = 1;
    uint64_t tmp = (uint64_t)n;
    int exp;

    /* factor 2 */
    exp = 0;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        exp++;
    }
    if (exp)
        result *= local_factor(2, exp);

    /* odd factors */
    for (uint64_t i = 3; i * i <= tmp; i += 2) {
        if (tmp % i == 0) {
            exp = 0;
            while (tmp % i == 0) {
                tmp /= i;
                exp++;
            }
            result *= local_factor(i, exp);
        }
    }

    /* remaining prime > sqrt(n) */
    if (tmp > 1)
        result *= local_factor(tmp, 1);

    return result;
}
