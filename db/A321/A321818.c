/*
 * A321818  a(n) = Sum_{d|n, n/d odd} d^8 for n > 0.
 * https://oeis.org/A321818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n, n/d odd} d^8 Equivalent multiplicative
 *            description: Let n = 2^k * m with m odd. Then a(n) = 2^(8k) *
 *            Prod_{p^e || m, p odd} (1 + p^8 + p^16 + ... + p^(8e))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer exponentiation by squaring for seq_t (unsigned integer) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* separate powers of 2 from the odd part */
    idx_t k = 0;
    idx_t temp = n;
    while ((temp & 1) == 0) {          /* while temp is even */
        temp >>= 1;
        ++k;
    }
    idx_t m = temp;                    /* odd part */

    /* contribution from odd part */
    seq_t result = 1;
    if (m > 1) {
        /* trial division for odd factors */
        idx_t p = 3;
        while (p <= temp / p) {
            if (temp % p == 0) {
                /* exponent e of prime p in m */
                idx_t e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }
                /* compute term = 1 + p^8 + p^16 + ... + p^(8e) */
                seq_t p8 = pow_seq((seq_t)p, 8);  /* p^8 */
                seq_t pow_val = 1;                /* p^(8*0) */
                seq_t term = 1;                   /* start with 1 */
                for (idx_t i = 1; i <= e; ++i) {
                    pow_val = pow_val * p8;      /* p^(8*i) */
                    term = term + pow_val;       /* accumulate sum */
                }
                result = result * term;           /* multiply into overall product */
            }
            p += 2;
        }
        /* any remaining prime factor > sqrt(original m) */
        if (temp > 1) {                       /* prime factor with exponent 1 */
            seq_t p = temp;
            seq_t term = 1 + pow_seq(p, 8);    /* 1 + p^8 */
            result = result * term;
        }
    }

    /* contribution from powers of 2: 2^(8k) */
    seq_t two_power = pow_seq((seq_t)2, 8 * k);
    result = result * two_power;

    return result;
}
