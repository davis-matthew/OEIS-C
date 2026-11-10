/*
 * A294932  Multiplicative with a(p^e) = A019565(A289814(e)).
 * https://oeis.org/A294932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e || n} A019565(A289814(e)); multiplicative,
 *            a(1) = 1
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9761 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A019565_primes[16] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53
};

static seq_t A019565_a(idx_t n)
{
    seq_t result = 1;
    idx_t bit = 0;
    while (n != 0) {
        if (n & 1ULL) {
            result *= (seq_t)A019565_primes[bit];
        }
        n >>= 1;
        ++bit;
    }
    return result;
}

static idx_t A289814_a(idx_t n)
{
    idx_t res = 0;
    idx_t pos = 0;
    while (n > 0) {
        if (n % 3 == 2)
            res |= ((idx_t)1 << pos);
        n /= 3;
        pos++;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t p;
    for (p = 2; p <= n / p; p++) {
        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            result *= A019565_a(A289814_a(e));
        }
    }
    if (n > 1) {
        /* remaining prime with exponent 1: A289814(1)=0 -> A019565(0)=1 */
        result *= A019565_a(A289814_a(1));
    }
    return result;
}
