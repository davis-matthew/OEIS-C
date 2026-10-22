/*
 * A319858  a(n) is the number of values of m in the interval [2,10] such that
 *          the base-m expansion of n, interpreted as a base-10 number, yields
 *          a prime.
 * https://oeis.org/A319858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of bases m ∈ [2,10] for which the base‑m expansion of n,
 * interpreted as a decimal integer, is prime. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static bool expansion_is_prime(idx_t n, unsigned base)
{
    /* Convert n to base 'base' (least‑significant digit first). */
    unsigned char digits[80];
    idx_t tmp = n;
    unsigned dcnt = 0;

    if (tmp == 0) {
        digits[dcnt++] = 0;
    } else {
        while (tmp > 0) {
            digits[dcnt++] = (unsigned char)(tmp % base);
            tmp /= base;
        }
    }

    /* Build the decimal integer from the most‑significant digit first. */
    uint64_t val64 = 0;
    for (int i = (int)dcnt - 1; i >= 0; --i) {
        val64 = val64 * 10 + digits[i];
    }

    /* Cast to seq_t (unsigned) and test primality. */
    seq_t val = (seq_t)val64;
    return is_prime_big(val);
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (unsigned base = 2; base <= 10; ++base) {
        if (expansion_is_prime(n, base)) {
            count++;
        }
    }
    return count;
}
