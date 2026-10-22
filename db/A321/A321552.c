/*
 * A321552  a(n) = Sum_{d|n} (-1)^(n/d+1)*d^7.
 * https://oeis.org/A321552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1)*d^7 The function implements the
 *            multiplicative formula: a(2^e) = (63*2^(7e+1)+1)/127 a(p^e) =
 *            (p^(7e+7)-1)/(p^7-1) for odd primes p
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 565,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: multiply two seq_t numbers (may be arbitrary precision). */
static inline seq_t mul_seq(seq_t a, seq_t b)
{
    return a * b;
}

/* Helper: compute base^exp with small integer exp (exp <= 428). */
static inline seq_t pow_int_seq(seq_t base, unsigned exp)
{
    seq_t result = 1;
    while (exp--) result = mul_seq(result, base);
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside the OEIS offset, not used */
    seq_t result = 1;

    idx_t temp = n;
    /* handle factor 2 separately */
    unsigned e2 = 0;
    while ((temp & 1ULL) == 0 && temp != 0) {
        ++e2;
        temp >>= 1;
    }
    if (e2 > 0) {
        /* compute 2^(7*e2+1) */
        idx_t exp2 = 7 * e2 + 1;
        seq_t pow2 = pow_int_seq((seq_t)2, (unsigned)exp2);
        seq_t numerator = mul_seq(pow2, (seq_t)63) + 1;
        seq_t val2 = numerator / 127;   /* exact division */
        result = mul_seq(result, val2);
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p != 0) continue;
        unsigned exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        /* compute p^7 */
        seq_t base = (seq_t)p;
        seq_t pow_p7 = pow_int_seq(base, 7);
        /* geometric sum: 1 + p^7 + p^14 + ... + p^(7*exp) */
        seq_t sum = 1;
        seq_t power = 1;
        for (unsigned k = 1; k <= exp; ++k) {
            power = mul_seq(power, pow_p7);
            sum += power;
        }
        result = mul_seq(result, sum);
    }

    /* remaining prime factor > sqrt(temp) */
    if (temp > 1) {
        seq_t base = (seq_t)temp;          /* temp is prime */
        /* exp = 1 */
        seq_t pow_p7 = pow_int_seq(base, 7);
        seq_t sum = 1 + pow_p7;            /* 1 + p^7 */
        result = mul_seq(result, sum);
    }

    return result;
}
