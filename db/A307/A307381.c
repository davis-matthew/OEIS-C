/*
 * A307381  Number of sextic primitive Dirichlet characters modulo n.
 * https://oeis.org/A307381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 9425, u16 n <= 9425, u32 n <= 9425, u64 n <= 9425,
 *            u128 n <= 9425, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A307381: Number of sextic primitive Dirichlet characters modulo n
   Formula: multiplicative with prime power values
   a(1)=1; a(2)=0, a(4)=1, a(8)=2; a(2^e)=0 for e=1 or e>=4
   a(3)=1, a(9)=4; a(3^e)=0 for e>=3
   a(p)=5 if p≡1 (mod 6), a(p)=1 if p≡5 (mod 6); a(p^e)=0 for p>3 and e≥2
*/

#include <stdint.h>
#include <stdbool.h>

/* Local integer square‑root: floor(sqrt(x)) */
static idx_t int_sqrt(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) <= x / (r + 1)) {
        ++r;
    }
    return r;
}

/* Value contributed by a prime power p^e */
static seq_t prime_power_value(uint64_t p, uint64_t e)
{
    if (p == 2) {
        switch (e) {
            case 1: return (seq_t)0;   /* a(2)=0 */
            case 2: return (seq_t)1;   /* a(4)=1 */
            case 3: return (seq_t)2;   /* a(8)=2 */
            default: return (seq_t)0;  /* e>=4 => 0 */
        }
    }
    if (p == 3) {
        switch (e) {
            case 1: return (seq_t)1;   /* a(3)=1 */
            case 2: return (seq_t)4;   /* a(9)=4 */
            default: return (seq_t)0;  /* e>=3 => 0 */
        }
    }
    /* p > 3 */
    if (e >= 2) return (seq_t)0;       /* a(p^e)=0 for e>=2 */
    /* e == 1 */
    return (p % 6 == 1) ? (seq_t)5 : (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1)=1 (offset) */
    idx_t temp = n;
    seq_t result = (seq_t)1;

    /* factor 2 separately to speed up */
    if (temp % 2 == 0) {
        uint64_t exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            ++exp;
        }
        seq_t v = prime_power_value(2, exp);
        if (v == (seq_t)0) return (seq_t)0;
        result *= v;
    }

    /* odd factors */
    for (idx_t i = 3; i <= int_sqrt(temp); i += 2) {
        if (temp % i == 0) {
            uint64_t exp = 0;
            while (temp % i == 0) {
                temp /= i;
                ++exp;
            }
            seq_t v = prime_power_value(i, exp);
            if (v == (seq_t)0) return (seq_t)0;
            result *= v;
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (temp > 1) {
        seq_t v = prime_power_value(temp, 1);
        if (v == (seq_t)0) return (seq_t)0;
        result *= v;
    }

    return result;
}
