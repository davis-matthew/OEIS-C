/*
 * A322849  Number of times 2^k (for k < 4) appears as a substring within 2^n.
 * https://oeis.org/A322849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of times the decimal digits 1,2,4,8 appear in the
 *            decimal representation of 2^n. The function computes 2^n in
 *            base‑10 by repeated doubling using an array of decimal digits.
 *            Then it scans the digits and counts those equal to 1,2,4 or 8.
 *            The allocation size is bounded by ceil(4n/13)+1...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Ensure enough space for the decimal digits of 2^n.
       Since log10(2) < 4/13 ≈ 0.3077, 4n/13 + 10 is always a
       safe upper bound on the number of decimal digits. */
    size_t est = (size_t)(n * 4 / 13) + 10;

    /* little‑endian decimal array, each element 0–9 */
    uint8_t *digits = (uint8_t *)malloc(est);
    if (!digits) return 0;                /* unlikely for given limits */

    size_t len = 1;          /* current number of digits */
    digits[0] = 1;           /* 2^0 = 1 */

    for (idx_t i = 0; i < n; ++i)
    {
        uint8_t carry = 0;
        for (size_t j = 0; j < len; ++j)
        {
            uint16_t tmp = (uint16_t)digits[j] * 2 + carry;
            digits[j] = (uint8_t)(tmp % 10);
            carry    = (uint8_t)(tmp / 10);
        }
        while (carry)
        {
            digits[len++] = carry % 10;
            carry /= 10;
        }
    }

    seq_t count = 0;
    for (size_t j = 0; j < len; ++j)
    {
        switch (digits[j])
        {
            case 1: case 2: case 4: case 8:
                ++count;
                break;
            default:
                break; /* nothing */
        }
    }

    free(digits);
    return count;
}
