/*
 * A306979  a(1) = 0; a(n) is the sum of powers of 2 where the exponents are
 *          the digits of a(n-1).
 * https://oeis.org/A306979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   get_digits, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; a(n)=sum_{digit d of a(n-1)}2^d   for n>1.
   A period of 15 occurs: a(n)=a(n-15) for all n≥36. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_digits.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static int init = 0;
    static seq_t cache[36];   /* indices 1..35 used */

    if (!init) {
        cache[1] = 0;
        for (idx_t i = 2; i <= 35; i++) {
            seq_t prev = cache[i - 1];

            uint8_t digits[64];
            size_t len;
            get_digits(prev, digits, &len);

            seq_t sum = 0;
            for (size_t j = 0; j < len; j++) {
                sum += pow2((idx_t)digits[j]);
            }
            cache[i] = sum;
        }
        init = 1;
    }

    if (n <= 35) return cache[n];
    while (n > 35) n -= 15;   /* reduce by the period */
    return cache[n];
}
