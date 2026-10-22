/*
 * A348434  Decimal expansion of (1/3)*e in Coulombs, one third of the
 *          elementary charge.
 * https://oeis.org/A348434
 *
 * Offset:    -19    (a(-19) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) is the (n+19)-th decimal digit of (1/3)*1.602176634 =
 *            534058878/10^9; a(-19) is the first digit after the point
 * Range:     u8 none, u16 none, u32 none, u64 n <= -11, u128 n <= -11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Only needed for n >= -19; other values are undefined but
       return 0 for safety. */
    if (n < -19) return 0;

    /* r = (1/3)*1.602176634 = 534058878 / 10^9 */
    static const seq_t NUM = 534058878ULL;   /* numerator          */
    static const seq_t DEN = 1000000000ULL;  /* denominator (10^9) */

    /* Lazy caching of decimal digits */
    static size_t  digits_len = 0;   /* how many digits we have computed  */
    static size_t  digits_cap = 0;   /* current capacity of the array      */
    static uint8_t *digits  = NULL;  /* array of decimal digits (0‑9)      */
    static seq_t   rem     = 0;     /* current remainder in long division */

    /* First call: allocate a small initial buffer. */
    if (!digits) {
        digits_cap = 16;
        digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        if (!digits) return 0;               /* out of memory → safe fallback */
        rem = NUM;                           /* start with the numerator        */
    }

    /* Position in the decimal expansion (0‑based). */
    size_t pos = (size_t)(n + 19);

    /* Generate all digits up to the requested position. */
    while (digits_len <= pos) {
        /* Long‑division step: multiply remainder by 10 then extract a digit. */
        rem *= 10;
        seq_t digit = rem / DEN;
        rem = rem % DEN;

        /* Resize array if necessary. */
        if (digits_len >= digits_cap) {
            digits_cap *= 2;
            uint8_t *tmp = (uint8_t *)realloc(digits, digits_cap);
            if (!tmp) {
                free(digits);
                return 0; /* memory error */
            }
            digits = tmp;
        }
        digits[digits_len++] = (uint8_t)digit;
        /* When rem becomes zero, all following digits will be zero;
           the loop will simply append zeros automatically. */
    }

    return (seq_t)digits[pos];
}
