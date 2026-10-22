/*
 * A368866  The smallest positive number such that 2^a(n) when written in base
 *          n contains adjacent equal digits.
 * https://oeis.org/A368866
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest k>0 such that the base‑n representation of 2^k
 *            contains at least one pair of adjacent equal digits
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0; /* offset starts at 2, behaviour below is irrelevant */

    const uint32_t base = (uint32_t)n;          /* base for digit conversion */
    /* Array of digits of 2^k in base ‘base’. Least significant digit first. */
    static uint32_t digits[4000];
    size_t len = 1;                 /* current length of digits array */
    digits[0] = 1;                  /* start from 2^0 = 1 */

    for (idx_t k = 1; ; ++k)        /* iterate k = 1,2,3,... */
    {
        /* Multiply the current number by 2 in base ‘base’. */
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i)
        {
            uint64_t temp = (uint64_t)digits[i] * 2 + carry;
            digits[i] = (uint32_t)(temp % base);
            carry = temp / base;
        }
        if (carry != 0)                      /* add a new most‑significant digit */
        {
            digits[len++] = (uint32_t)carry;
        }

        /* Check for adjacent equal digits. */
        for (size_t i = 0; i + 1 < len; ++i)
        {
            if (digits[i] == digits[i + 1])
                return (seq_t)k;            /* found the smallest k */
        }
    }
}
