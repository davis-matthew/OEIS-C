/*
 * A259081  a(n) = largest k such that the decimal representation of 3^k does
 *          not contain the digit n.
 * https://oeis.org/A259081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = largest k such that decimal representation of 3^k does
 *            not contain the digit n (0‑9). The function computes the needed
 *            powers of 3 using a decimal digit array to avoid 64‑bit overflow
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Only digits 0‑9 are meaningful; otherwise return 0. */
    if (n >= 10)
        return (seq_t)0;

    /* Lazily compute the sequence once. */
    static bool ready = false;
    static idx_t last_k[10];
    static uint8_t *digits = NULL;   /* decimal digits, least significant first */
    static size_t digits_len = 0;      /* current number of digits */
    static size_t digits_cap = 0;      /* allocated capacity */

    if (!ready) {
        /* --------------------------------------------------------------------
         *  Initialise the digit array to hold 3^0 == 1 and start the search.
         * -------------------------------------------------------------------- */
        const idx_t MAX_K = 12000;            /* large enough for all known values */

        /* Allocate a reasonable initial capacity. */
        digits_cap = 64;
        digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        digits[0] = 1;
        digits_len = 1;

        /* Initialise last_k with sentinel values. */
        for (size_t i = 0; i < 10; ++i)
            last_k[i] = (idx_t)-1;

        /* --------------------------------------------------------------------
         *  Loop over exponents k, updating the decimal representation of 3^k.
         *  For each k we check which digits are absent and record the largest
         *  k for each digit accordingly.
         * -------------------------------------------------------------------- */
        for (idx_t k = 0; k <= MAX_K; ++k) {
            /* Determine which digits are present in the current number. */
            bool present[10] = {false};
            for (size_t i = 0; i < digits_len; ++i)
                present[digits[i]] = true;

            /* Update last_k for digits that are absent. */
            for (int d = 0; d < 10; ++d)
                if (!present[d])
                    last_k[d] = k;

            /* Compute next power (3^(k+1)) by multiplying by 3. */
            if (k == MAX_K)   /* no need to multiply after final iteration */
                break;
            uint16_t carry = 0;
            for (size_t i = 0; i < digits_len; ++i) {
                uint16_t prod = (uint16_t)digits[i] * 3 + carry;
                digits[i]   = (uint8_t)(prod % 10);
                carry       = prod / 10;
            }
            while (carry > 0) {
                if (digits_len >= digits_cap) {
                    digits_cap *= 2;
                    digits = (uint8_t *)realloc(digits, digits_cap * sizeof(uint8_t));
                }
                digits[digits_len++] = (uint8_t)(carry % 10);
                carry /= 10;
            }
        }

        ready = true;
    }

    return (seq_t)last_k[n];
}
