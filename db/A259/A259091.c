/*
 * A259091  Smallest k such that 2^k contains two adjacent copies of n in its
 *          decimal expansion.
 * https://oeis.org/A259091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest k such that 2^k contains two adjacent
 *            copies of the decimal representation of n. This implementation
 *            performs a brute‑force search by repeatedly doubling the value
 *            and scanning the decimal digits for the required pattern
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_pattern, contains_pattern, multiply_by_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/build_pattern.h"
#include "../utils/contains_pattern.h"
#include "../utils/multiply_by_two.h"

seq_t a(idx_t n)
{
    uint8_t *pat;
    size_t plen;
    build_pattern(n, &pat, &plen);

    size_t cap = 32;
    uint8_t *digits = (uint8_t *)malloc(cap);
    digits[0] = 1;
    size_t dlen = 1;

    seq_t exponent = 0;

    while (true)
    {
        if (contains_pattern(digits, dlen, pat, plen))
        {
            free(pat);
            free(digits);
            return exponent;
        }

        if (dlen + 1 > cap)
        {
            cap <<= 1;
            digits = (uint8_t *)realloc(digits, cap);
        }

        multiply_by_two(digits, &dlen);
        exponent++;
    }
}
