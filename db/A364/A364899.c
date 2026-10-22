/*
 * A364899  Decimal expansion of the number of grams per cubic centimeter
 *          (g/cm^3) in 1 ounce per cubic inch (oz/in^3).
 * https://oeis.org/A364899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) gives the n-th decimal digit of the rational number
 *            28.349523125 / 2.54^3. Let N = 28349523125 * 125000 =
 *            3543690390625000 D = 1000000000 * 2048383 = 2048383000000000
 *            then the number is N / D. a(1) = floor(N / D); for n > 1, the
 *            digits are produced by long division: r_{0} = N % D; for
 *            i>=1,...
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const uint64_t N64 = 3543690390625000ULL;
static const uint64_t D64 = 2048383000000000ULL;

static seq_t *digits = NULL;
static size_t digits_cap = 0;
static size_t digits_len = 0;
static uint64_t rem64 = 0;      /* remainder for fractional part */

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1, safe guard */

    /* initialise on first call */
    if (digits_len == 0) {
        uint64_t int_part = N64 / D64;
        rem64 = N64 % D64;
        digits_cap = 32;
        digits = (seq_t*)malloc(digits_cap * sizeof(seq_t));
        digits[0] = (seq_t)int_part;
        digits_len = 1;
    }

    /* if requested digit is already cached, return it */
    if ((size_t)n <= digits_len) {
        return digits[n - 1];
    }

    /* extend cache to at least n digits */
    for (size_t i = digits_len; i < (size_t)n; ++i) {
        if (digits_len >= digits_cap) {     /* grow buffer */
            digits_cap *= 2;
            digits = (seq_t*)realloc(digits, digits_cap * sizeof(seq_t));
        }
        uint64_t prod = rem64 * 10ULL;       /* up to ~2e16, fits in 64‑bit */
        uint64_t digit = prod / D64;
        rem64 = prod % D64;
        digits[digits_len++] = (seq_t)digit;
    }

    return digits[n - 1];
}
