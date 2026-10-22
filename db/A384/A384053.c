/*
 * A384053  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is a unitary divisor of n is a cube.
 * https://oeis.org/A384053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e || n} ( p^e if e ≡ 0 (mod 3) else (p^e - 1) )
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Local power function using only +, -, *, /, %, <<, >> */
static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;      /* empty product */
    seq_t result = 1;
    idx_t temp = n;
    /* factor 2 separately */
    if ((temp & 1) == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        seq_t pe = pow_seq((seq_t)2, e);
        if ((e % 3) == 0)
            result *= pe;
        else
            result *= (pe - 1);
    }
    /* odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t pe = pow_seq((seq_t)p, e);
            if ((e % 3) == 0)
                result *= pe;
            else
                result *= (pe - 1);
        }
    }
    /* remaining prime factor, if any */
    if (temp > 1) {
        seq_t pe = (seq_t)temp;  /* p^1 */
        result *= (pe - 1);      /* exponent 1 is not divisible by 3 */
    }
    return result;
}
