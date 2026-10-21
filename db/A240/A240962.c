/*
 * A240962  Number of zeros in the decimal expansion of n^n.
 * https://oeis.org/A240962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of zeros in the decimal expansion of n^n
 * Range:     u8 n <= 199, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* out of range for OEIS offset, but safe */
        return (seq_t)0;

    /* Base 10^9 representation: each limb fits in 32 bits, multiplication
       by n (up to 10 000) stays within 64 bits. */
    const uint32_t BASE = 1000000000U;

    size_t cap = 64;                     /* initial capacity of limb array */
    uint32_t *digits = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!digits)
        return (seq_t)0;

    size_t len = 1;                      /* current number of limbs */
    digits[0] = 1;                       /* start with 1 (i.e., n^0) */

    /* Compute n^n by multiplying by n, n times. */
    for (idx_t i = 0; i < n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t prod = (uint64_t)digits[j] * (uint64_t)n + carry;
            digits[j] = (uint32_t)(prod % BASE);
            carry     = prod / BASE;
        }
        while (carry) {                  /* propagate remaining carry */
            if (len == cap) {            /* grow the array if needed */
                cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(digits, cap * sizeof(uint32_t));
                if (!tmp) {
                    free(digits);
                    return (seq_t)0;
                }
                digits = tmp;
            }
            digits[len++] = (uint32_t)(carry % BASE);
            carry /= BASE;
        }
    }

    /* Count zeros in the decimal representation. */
    uint64_t zeros = 0;
    for (size_t i = 0; i < len; ++i) {
        uint32_t val = digits[i];
        if (i == len - 1) {               /* most significant limb – no leading zeros */
            if (val == 0)
                zeros++;                  /* not possible for n ≥ 1 */
            while (val) {
                if ((val % 10) == 0)
                    zeros++;
                val /= 10;
            }
        } else {                           /* lower limbs are always 9 decimal digits */
            for (int k = 0; k < 9; ++k) {
                if ((val % 10) == 0)
                    zeros++;
                val /= 10;
            }
        }
    }

    free(digits);
    return (seq_t)zeros;
}
