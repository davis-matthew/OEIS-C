/*
 * A344135  a(n) = Sum_{i|n, j|n, k|n} i*j*k/lcm(i,j,k).
 * https://oeis.org/A344135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{i|n, j|n, k|n} i*j*k/lcm(i,j,k)
 * Range:     u8 n <= 5, u16 n <= 59, u32 n <= 9239, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Contribution of a prime power p^e to the multiplicative function a(n) */
static seq_t prime_power_contribution(seq_t p, int e)
{
    int max_exp = 2 * e;                       /* maximum exponent needed */
    seq_t *pow_arr = (seq_t *)malloc((max_exp + 1) * sizeof(seq_t));
    pow_arr[0] = (seq_t)1;
    for (int i = 1; i <= max_exp; ++i)
        pow_arr[i] = pow_arr[i - 1] * p;

    seq_t sum = (seq_t)0;
    for (int e1 = 0; e1 <= e; ++e1) {
        for (int e2 = 0; e2 <= e; ++e2) {
            for (int e3 = 0; e3 <= e; ++e3) {
                int maxe = e1;
                if (e2 > maxe) maxe = e2;
                if (e3 > maxe) maxe = e3;
                int exponent = e1 + e2 + e3 - maxe;   /* sum of the two smallest exponents */
                sum += pow_arr[exponent];
            }
        }
    }
    free(pow_arr);
    return sum;
}

/* OEIS sequence A344135 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, but keeps a(0)=0   */
    idx_t m = n;
    seq_t result = (seq_t)1;        /* product over prime powers */

    /* factor 2 separately */
    int exp = 0;
    while ((m & 1) == 0) {
        ++exp;
        m >>= 1;
    }
    if (exp)
        result *= prime_power_contribution((seq_t)2, exp);

    /* odd prime factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        int e = 0;
        while (m % p == 0) {
            ++e;
            m /= p;
        }
        if (e)
            result *= prime_power_contribution((seq_t)p, e);
    }

    /* remaining prime (if any) */
    if (m > 1)
        result *= prime_power_contribution((seq_t)m, 1);

    return result;
}
