/*
 * A249549  Conjectured largest number k not divisible by 10 such that k^k has
 *          exactly n distinct decimal digits.
 * https://oeis.org/A249549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A249549:
   a(n) = largest integer k not divisible by 10 such that k^k has exactly n
   distinct decimal digits.  The function uses a brute‑force search up to a
   fixed upper bound (200) which covers all known terms up to n = 9.  For
   n > 10 no such k exists. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a[1..10] holds the maximal k for each distinct‑digit count. */
    static seq_t a[11] = {0};
    static bool ready = false;
    const idx_t LIMIT = 200;          /* safe upper bound for all known a(n) */

    if (!ready)
    {
        uint8_t digits[2000];         /* decimal digits, least‑significant first */

        for (idx_t k = 1; k <= LIMIT; ++k)
        {
            if (k % 10 == 0) continue; /* skip multiples of 10 */

            /* compute k^k in decimal digits */
            int len = 1;
            digits[0] = 1;
            for (idx_t m = 0; m < k; ++m)
            {
                unsigned carry = 0;
                for (int j = 0; j < len; ++j)
                {
                    unsigned prod = (unsigned)digits[j] * (unsigned)k + carry;
                    digits[j] = prod % 10;
                    carry = prod / 10;
                }
                while (carry)
                {
                    digits[len++] = carry % 10;
                    carry /= 10;
                }
            }

            /* distinct‑digit mask */
            uint16_t mask = 0;
            for (int j = 0; j < len; ++j)
                mask |= 1u << digits[j];

            int distinct = 0;
            for (int b = 0; b < 10; ++b)
                if (mask & (1u << b)) ++distinct;

            if (distinct >= 1 && distinct <= 10 && k > a[distinct])
                a[distinct] = (seq_t)k;
        }
        ready = true;
    }

    if (n >= 1 && n <= 10)
        return a[n];
    return (seq_t)0;
}
