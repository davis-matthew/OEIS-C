/*
 * A398964  Number of solutions of x^3 + y^3 = z^2 in (Z/nZ)^3.
 * https://oeis.org/A398964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   a_pow_2, a_pow_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A398964
 *  Sequence: Number of solutions of x^3 + y^3 = z^2 in (Z/nZ)^3
 *  a(n) is multiplicative.
 *  • For primes p >= 5 :
 *      a(p^k) = p^(2k-2) * (p^2 + p - 1)   (k >= 2)
 *      a(p)   = p^2
 *  • For p = 2 and p = 3 the values are given by the recurrences
 *        a(2^k) :  a(0)=1,
 *            a(k)=4*a(k-1)+2048*a(k-6)-8192*a(k-7)+R(k),
 *        R(2)=4, R(6)=-1024, otherwise 0.
 *        a(3^k) :  a(0)=1,
 *            a(k)=9*a(k-1)+177147*a(k-6)-1594323*a(k-7)+S(k),
 *        S(2)=54, S(6)=-59049, otherwise 0.
 *
 *  The function factors n by trial division (fast enough for n ≤ 10000)
 *  and multiplies the contributions of the prime powers.
 */

#include <stdint.h>
#include "../utils/a_pow_2.h"
#include "../utils/a_pow_3.h"

/* Small integer power for seq_t base and idx_t exponent */
static seq_t
seq_pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (idx_t)1)
            result *= base;
        exp >>= 1;
        if (exp == 0)
            break;
        base *= base;
    }
    return result;
}

/* a(p^k) for primes p >= 5 */
static seq_t
a_prime_power(uint64_t p, idx_t k)
{
    if (k == 0)
        return (seq_t)1;
    if (k == 1)
        return seq_pow_uint((seq_t)p, (idx_t)2);  /* a(p) = p^2 */

    /* k >= 2 : a(p^k) = p^(2k-2) * (p^2 + p - 1) */
    seq_t pp  = (seq_t)p;
    seq_t pp2 = pp * pp;                 /* p^2 */
    seq_t mult = pp2 + pp - (seq_t)1;    /* p^2 + p - 1 */
    idx_t exp   = 2 * k - 2;              /* 2k-2, fits in idx_t */
    seq_t pow   = seq_pow_uint(pp, exp);  /* p^(2k-2) */
    return pow * mult;
}

/* Main sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, safe fallback */
    if (n == 1) return (seq_t)1;   /* a(1) = 1 */

    uint64_t m   = (uint64_t)n;    /* use 64‑bit for trial division */
    seq_t result = (seq_t)1;

    /* handle factor 2 */
    idx_t cnt2 = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++cnt2;
    }
    if (cnt2 > 0)
        result *= a_pow_2(cnt2);

    /* handle odd factors by trial division up to sqrt(m) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t k = 0;
            while (m % p == 0) {
                m /= p;
                ++k;
            }
            if (p == 3)
                result *= a_pow_3(k);
            else
                result *= a_prime_power(p, k);
        }
    }

    /* remaining prime factor > 1 (may be 3 or a large prime) */
    if (m > 1) {
        if (m == 3)
            result *= a_pow_3(1);
        else
            result *= a_prime_power(m, 1);
    }

    return result;
}
