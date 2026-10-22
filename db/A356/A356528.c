/*
 * A356528  Decimal expansion of number of millimeters of water (mmH2O) in 1
 *          standard atmosphere (atm).
 * https://oeis.org/A356528
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal expansion of number of millimeters of water (mmH2O) in
 * 1 standard atmosphere (atm).
 * Sequence A356528, offset 5.
 * The data are obtained from the rational number
 *   101325 / 9.80665   = 2895 * 10^5 / 28019
 * with a period of 28018 digits after the decimal point.
 * We precompute the 5 integer digits and the period of fractional
 * digits once, and answer queries in O(1).
 *
 * a(n) returns the nth digit in the decimal representation
 * (most-significant integer digit first).  For n < 5
 * the value is undefined (0 is returned).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* constants for the rational number */
static const idx_t P = 2895ULL * 100000ULL;  /* 289500000 */
static const idx_t Q = 28019ULL;

/* period of the fractional part */
static const idx_t PERIOD = 28018ULL;

/* digit array for the 5 integer digits (most‑significant first) */
static seq_t int_digits[5];

/* array holding one full period of the fractional digits */
static uint8_t *frac_digits = NULL;
static bool initialized = false;

/* initialize the digit tables the first time a() is called */
static void init_digits(void)
{
    /* compute integer part and remainder */
    idx_t integer_part = P / Q;
    idx_t r = P % Q;

    /* extract the 5 integer digits (most‑significant to least) */
    idx_t tmp = integer_part;
    for (int i = 4; i >= 0; --i) {
        int_digits[i] = (seq_t)(tmp % 10);
        tmp /= 10;
    }

    /* allocate space for the fractional period */
    frac_digits = (uint8_t *)malloc(PERIOD * sizeof(uint8_t));

    /* generate the fractional digits by long division */
    for (idx_t i = 0; i < PERIOD; ++i) {
        r *= 10;
        frac_digits[i] = (uint8_t)(r / Q);
        r %= Q;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    /* sequence offset: from n = 5 */
    if (n < 5) {
        return (seq_t)0;   /* undefined below the offset */
    }

    if (!initialized) {
        init_digits();
    }

    /* 0‑based index of the desired digit in the combined sequence */
    idx_t idx = n - 5;

    if (idx < 5) {
        /* integer digits (positions 0–4) */
        return (seq_t)int_digits[(int)idx];
    } else {
        /* fractional digits start from index 5 */
        idx_t frac_idx = (idx - 5) % PERIOD;
        return (seq_t)frac_digits[(size_t)frac_idx];
    }
}
