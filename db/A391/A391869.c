/*
 * A391869  Delete from n all digits of opposite parity to its largest digit.
 * https://oeis.org/A391869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = delete from n all digits of opposite parity to its
 *            largest digit
 * Range:     u8 n <= 259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Extract the decimal digits of n in reverse order */
    uint8_t digits[21];   /* enough for any 64‑bit integer */
    size_t len = 0;

    if (n == 0)
    {
        digits[0] = 0;
        len = 1;
    }
    else
    {
        idx_t tmp = n;
        while (tmp != 0)
        {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    /* Find the largest digit */
    uint8_t largest = 0;
    for (size_t i = 0; i < len; ++i)
        if (digits[i] > largest)
            largest = digits[i];

    /* Parity to keep: 0 for even, 1 for odd */
    int keepParity = largest % 2;

    /* Reconstruct the result from most significant to least significant digit */
    seq_t res = 0;
    for (size_t i = len; i-- > 0; )
        if ((digits[i] % 2) == keepParity)
            res = res * 10 + digits[i];

    return res;
}
