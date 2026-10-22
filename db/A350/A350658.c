/*
 * A350658  a(n) is the minimal digit, expressed in base 10, in the base-n
 *          representation of 2^n.
 * https://oeis.org/A350658
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min digit of the base‑n representation of 2^n
 * Range:     u8 n <= 1787, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* strictly speaking the sequence is defined for n >= 2; for n < 2
     * we return 1 to keep the function well‑defined. */
    if (n < 2) return (seq_t)1;

    /* The number of base‑n digits of 2^n never exceeds n, so a simple
     * buffer of that size is sufficient for all n up to the limits
     * required by the tests. */
    uint64_t *digits = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    if (!digits) return (seq_t)0;  /* unexpected: allocation failure */

    size_t len = 1;           /* current number of digits (LSB first) */
    digits[0] = 1ULL;         /* start with 2^0 = 1 in base n */
    const uint64_t mod = (uint64_t)n;

    /* Multiply the base‑n number by 2, n times, to obtain 2^n. */
    for (idx_t exp = 1; exp <= n; ++exp)
    {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j)
        {
            uint64_t temp = digits[j] * 2ULL + carry;
            digits[j] = temp % mod;
            carry   = temp / mod;
        }
        if (carry > 0)
        {
            /* grow the buffer if necessary (should not happen for n <= 10000) */
            if (len == n)
            {
                uint64_t *tmp = (uint64_t *)realloc(digits,
                                  (size_t)(n + 1) * sizeof(uint64_t));
                if (!tmp) { free(digits); return (seq_t)0; }
                digits = tmp;
            }
            digits[len++] = carry;
        }
    }

    /* Find the minimal digit among the base‑n digits. */
    seq_t min_digit = (seq_t)n;   /* any digit is < n */
    for (size_t j = 0; j < len; ++j)
    {
        seq_t d = (seq_t)digits[j];
        if (d < min_digit)
        {
            min_digit = d;
            if (min_digit == 0) break;
        }
    }

    free(digits);
    return min_digit;
}
