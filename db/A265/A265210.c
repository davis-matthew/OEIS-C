/*
 * A265210  Irregular triangle read by rows in which row n lists the base 3
 *          digits of 2^n in reverse order, n >= 0.
 * https://oeis.org/A265210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the (@(n)– cumulative_row_lengths)th digit (reversed) of the
 * base‑3 representation of 2^row, where row is the smallest
 * integer with cumulative_row_lengths(row) > n .
 * The digits of 2^row are produced in least‑significant‑digit first order
 * by repeatedly doubling the base‑3 number. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamically grow a buffer that holds the base‑3 digits of 2^row.
     * The digits are stored least‑significant first.                                      */
    uint8_t *digits = NULL;   /* buffer for digits */
    size_t   len = 1;         /* current number of digits (≥1) */
    size_t   capacity = 1;    /* allocated size of digits */
    digits = (uint8_t *)malloc(capacity * sizeof(uint8_t));
    if (!digits) return (seq_t)0; /* out of memory: return 0 (unlikely) */
    digits[0] = 1;            /* 2^0 = 1 in base 3 */

    idx_t cumulative = 0;     /* number of digits counted in earlier rows */

    for (;;)
    {
        /* Length of current row (number of base‑3 digits of 2^row).       */
        idx_t row_len = (idx_t)len;

        /* If n lies inside this row, return the corresponding digit.   */
        if (cumulative + row_len > n)
        {
            idx_t offset = n - cumulative;          /* offset within the row */
            seq_t result = (seq_t)digits[(size_t)offset];
            free(digits);
            return result;
        }

        /* Advance to the next row:  multiply the current base‑3 number by 2. */
        cumulative += row_len;
        uint8_t carry = 0;

        for (size_t i = 0; i < len; ++i)
        {
            uint32_t prod = (uint32_t)digits[i] * 2 + carry; /* product fits in 32 bits */
            digits[i] = (uint8_t)(prod % 3);
            carry = (uint8_t)(prod / 3);
        }

        /* Append carry digit if it exists.                                 */
        if (carry)
        {
            if (len + 1 > capacity)
            {
                size_t new_cap = capacity * 2 + 1;
                uint8_t *tmp = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
                if (!tmp) { free(digits); return (seq_t)0; }
                digits = tmp;
                capacity = new_cap;
            }
            digits[len] = carry;
            ++len;
        }
    }
}
