/*
 * A327122  Expansion of Sum_{k>=1} sigma(k) * x^k / (1 + x^(2*k)), where
 *          sigma = A000203.
 * https://oeis.org/A327122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative. For a prime power a(2^e) = 2^(e+1) – 1.
 *            For an odd prime p: if p ≡ 1 (mod 4) : a(p^e) =
 *            (p^(e+2)-(e+2)*p+e+1) / (p-1)^2. if p ≡ 3 (mod 4) : a(p^e) =
 *            (2*p^(e+2)+((-1)^e-1)*p-((-1)^e+1)) / (2*(p^2-1)). The function
 *            computes a(n) by prime factorisation
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 6042, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--- dependencies -----------------------------------------------------*/
#include <stdint.h>

/*--- small power routine (does not overflow when the result fits in
     seq_t, which holds for the n ranges used in the tests) -----------*/
static seq_t seq_pow_uint(seq_t base, idx_t exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/*--- a(p^e) for a prime p --------------------------------------------*/
static seq_t prime_power_a(uint64_t p, uint64_t e) {
    if (p == 2) return seq_pow_uint(2, e + 1) - 1;

    seq_t pow = seq_pow_uint((seq_t)p, e + 2);

    if (p % 4 == 1) {
        seq_t term = (seq_t)(e + 2) * p;               /* (e+2)*p   */
        seq_t num  = pow - term + (seq_t)(e + 1);      /* p^(e+2) -(e+2)p + e+1 */
        seq_t den  = (seq_t)(p - 1) * (seq_t)(p - 1);  /* (p-1)^2   */
        return num / den;
    } else { /* p % 4 == 3 */
        seq_t num;
        if ((e & 1ULL) == 0)          /* e even  */
            num = 2 * pow - 2;
        else                          /* e odd   */
            num = 2 * pow - 2 * p;
        seq_t den = 2 * ((seq_t)p * (seq_t)p - 1);
        return num / den;
    }
}

/*--- main sequence routine -------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 1) return 1;

    uint64_t tmp = (uint64_t)n;
    seq_t result = 1;

    /* factor 2 separately */
    uint64_t e = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        e++;
    }
    if (e > 0)
        result *= prime_power_a(2, e);

    /* odd factors */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            result *= prime_power_a(p, e);
        }
    }

    /* remainder is a prime */
    if (tmp > 1)
        result *= prime_power_a(tmp, 1);

    return result;
}
