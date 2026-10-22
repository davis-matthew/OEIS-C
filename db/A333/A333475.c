/*
 * A333475  Numbers k such that S(2^k) is a perfect square, where S(t) is the
 *          sum of decimal digits of t.
 * https://oeis.org/A333475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 81, u32 n <= 209, u64 n <= 209,
 *            u128 n <= 209, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A333475 :  Numbers k such that S(2^k) is a perfect square
    The function a(n) returns the n‑th element (1‑based).  The code
    builds the powers of two in decimal as a digit array, keeping the
    digit sum in a 64‑bit accumulator.  When the sum is a perfect
    square the current exponent k is stored in the cache.  The routine
    is fully internal; it works for any seq_t (built‑in integer or
    arbitrary‑precision) and for any idx_t up to at least 10000.  */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* ---------------  static state  ----------------- */
    static seq_t *k_cache   = NULL;   /* cached k values */
    static size_t  cache_len = 0;      /* number of cached terms */
    static size_t  cache_cap = 0;      /* allocated capacity (seq_t) */

    static uint8_t *digits   = NULL;   /* decimal digits of current 2^k */
    static size_t   digits_len = 0;    /* number of used digits   */
    static size_t   digits_cap = 0;    /* allocated capacity      */

    static uint64_t digit_sum = 0;     /* sum of decimal digits   */
    static idx_t    cur_k      = 0;     /* current exponent (k)    */

    /* -----------------  initialise  ----------------- */
    if (digits == NULL) {
        digits_cap = 4000;                     /* ample for 2^10000 (~3011 digits) */
        digits     = (uint8_t *)malloc(digits_cap);
        digits[0]  = 1;                        /* 2^0 = 1 */
        digits_len = 1;
        digit_sum  = 1;                        /* digit sum of 1 */
        cache_cap  = 16;
        k_cache    = (seq_t *)malloc(cache_cap * sizeof(seq_t));

        /* k = 0 satisfies the condition (digit sum = 1) */
        k_cache[0] = (seq_t)0;
        cache_len  = 1;
    }

    /* -----------------  generate terms  -------------- */
    while (cache_len < n) {
        /* double the decimal number 2^(cur_k) -> 2^(cur_k+1) */
        uint8_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            uint8_t old = digits[i];
            uint16_t tmp = (uint16_t)old * 2 + carry;
            uint8_t newd = tmp % 10;
            carry = tmp / 10;
            digits[i] = newd;
            digit_sum += (uint64_t)newd - (uint64_t)old;   /* adjust sum */
        }
        if (carry) {                       /* overflow into a new most‑significant digit */
            if (digits_len >= digits_cap) {
                digits_cap *= 2;
                uint8_t *tmp = (uint8_t *)realloc(digits, digits_cap);
                if (tmp == NULL) exit(EXIT_FAILURE);
                digits = tmp;
            }
            digits[digits_len++] = carry;
            digit_sum += carry;           /* new digit contributes to the sum */
        }

        /* new exponent after the multiplication */
        ++cur_k;

        /* check if digit sum is a perfect square */
        seq_t sum   = (seq_t)digit_sum;
        seq_t r     = floor_sqrt(sum);
        if (r * r == sum) {              /* perfect square */
            if (cache_len >= cache_cap) { /* grow cache */
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(k_cache, cache_cap * sizeof(seq_t));
                if (tmp == NULL) exit(EXIT_FAILURE);
                k_cache = tmp;
            }
            k_cache[cache_len++] = (seq_t)cur_k;
        }
    }

    /* -----------------  return result  -------------- */
    return k_cache[n - 1];    /* 1‑based indexing */
}
