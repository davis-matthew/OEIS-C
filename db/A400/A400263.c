/*
 * A400263  The second least significant digit of n' [the arithmetic
 *          derivative of n, A003415] in the primorial base, A049345.
 * https://oeis.org/A400263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = ( ( n' / 2 ) mod 3 ), 
 * where n' is the arithmetic derivative of n.
 * n' is computed as n' = Σ (n / p_i) * e_i over all prime powers p_i^e_i dividing n.
 * For n ≤ 1 we return 0.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 0 and 1 give derivative 0 */
    if (n <= 1) return (seq_t)0;

    uint64_t n64 = (uint64_t)n;          /* input fits in 64‑bit */
    seq_t derivative = 0;

    uint64_t temp = n64;

    /* handle factor 2 */
    uint64_t exp = 0;
    while ((temp & 1ULL) == 0) {  /* temp % 2 == 0 */
        temp >>= 1;               /* temp /= 2 */
        exp++;
    }
    if (exp > 0) {
        derivative += ((seq_t)n / (seq_t)2) * exp;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            derivative += ((seq_t)n / (seq_t)p) * exp;
        }
    }

    /* remaining prime factor > 1 */
    if (temp > 1) {
        derivative += ((seq_t)n / (seq_t)temp);  /* exponent is 1 */
    }

    /* compute the required digit: (n'/2) mod 3 */
    seq_t result = (derivative / (seq_t)2) % (seq_t)3;
    return result;
}
