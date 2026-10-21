/*
 * A252482  Exponents n such that the decimal expansion of the power 12^n
 *          contains no zeros.
 * https://oeis.org/A252482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   grow_digits, grow_exps, has_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth exponent n such that 12^n has no decimal digit 0.
   The function generates successive powers of 12 using a manual
   base‑10⁹ big integer representation and records all exponents
   whose decimal expansion contains no zero.  The result is memoised
   in a static array so subsequent calls (in any order) take
   O(1) after the needed exponents have been computed.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/grow_digits.h"
#include "../utils/grow_exps.h"
#include "../utils/has_zero.h"

/* ------------------------------------------------------------------ */
/* Big integer represented as an array of 32‑bit blocks in base 1e9.  */
/* ------------------------------------------------------------------ */
#define BASE 1000000000u  /* 10^9 */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */

    /* ----------------- static state --------------------------------- */
    static seq_t  *exps       = NULL;
    static size_t  exps_len   = 0;
    static size_t  exps_cap   = 0;

    static uint32_t *digits    = NULL;   /* base‑1e9 big integer */
    static size_t    digits_len = 0;
    static size_t    digits_cap = 0;

    static idx_t    cur_exp = 0;  /* exponent of the current big integer */

    /* ----------------- initialise once ------------------------------ */
    if (exps_len == 0) {                /* first call */
        /* exponents storage */
        exps       = NULL;
        exps_cap   = 0;

        /* big integer storage – start with 12^0 = 1 */
        digits    = NULL;
        digits_cap = 0;
        digits_len  = 0;
        grow_digits(&digits, &digits_cap, 4);
        digits[0] = 1;
        digits_len = 1;
        cur_exp = 0;
    }

    /* ----------------- generate until we have n exponents ------------- */
    while (exps_len < (size_t)n) {

        /* 1. test current power for a zero digit */
        if (!has_zero(digits, digits_len)) {
            grow_exps(&exps, &exps_cap, exps_len + 1);
            exps[exps_len++] = (seq_t)cur_exp;
        }

        /* 2. compute next power: multiply by 12 */
        uint64_t carry = 0;
        size_t i;
        for (i = 0; i < digits_len; ++i) {
            uint64_t prod = (uint64_t)digits[i] * 12u + carry;
            digits[i] = (uint32_t)(prod % BASE);
            carry = prod / BASE;
        }
        while (carry > 0) {
            grow_digits(&digits, &digits_cap, digits_len + 1);
            digits[digits_len++] = (uint32_t)(carry % BASE);
            carry /= BASE;
        }

        ++cur_exp;
    }

    /* ----------------- answer for requested index ------------------- */
    return exps[n - 1];
}
