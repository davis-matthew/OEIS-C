/*
 * A261010  Write 5^n in base 3, add up the "digits".
 * https://oeis.org/A261010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of base‑3 digits of 5^n
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate enough space: roughly 1.465*n base‑3 digits,
       double that to be safe. */
    size_t cap = 2 * (size_t)n + 10;
    uint8_t *d = (uint8_t *)malloc(cap);
    if (d == NULL)
        return (seq_t)0; /* allocation failure; unlikely in test suite */

    size_t len = 1;   /* current number of digits (least significant first) */
    d[0] = 1;         /* 5^0 = 1 */

    /* Multiply by 5, n times, keeping the representation in base 3. */
    for (idx_t i = 0; i < n; ++i) {
        uint8_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint16_t prod = (uint16_t)d[j] * 5 + carry; /* prod <= 2*5+4 = 14 */
            d[j]   = (uint8_t)(prod % 3);
            carry  = (uint8_t)(prod / 3);
        }
        while (carry > 0) {                     /* propagate any remaining carry */
            if (len == cap) {                   /* double capacity if needed */
                cap <<= 1;
                d = (uint8_t *)realloc(d, cap);
                if (d == NULL)
                    return (seq_t)0;           /* allocation failure */
            }
            d[len++] = carry % 3;
            carry   /= 3;
        }
    }

    /* Sum the base‑3 digits. */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += d[i];

    free(d);
    return sum;
}
