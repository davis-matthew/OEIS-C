/*
 * A307769  Decimal expansion of 2^(-149).
 * https://oeis.org/A307769
 *
 * Offset:    -44    (a(-44) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = decimal digit of 5^149 starting with most significant
 *            digit at n = -44 (sequence a(-44) = 1, a(-43) = 4, …)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute 5^149 in base‑10 using an array of decimal digits and
   store the result in a static array.  For n < -44 the behavior is
   unspecified; for n ≥ -44 we return the corresponding digit (or 0
   if the index is beyond the 105 available digits). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* static data: digits of 5^149 (105 digits) in big‑endian order */
    static unsigned char digits[105];
    static int initialized = 0;

    if (!initialized) {
        /* Compute 5^149 using decimal digit array.  Use little‑endian
           representation for intermediate calculations. */
        unsigned char big[200] = {0};   /* padding to be safe */
        int len = 1;                    /* current number of digits */
        big[0] = 1;                     /* start with 1 */

        for (int mul = 0; mul < 149; ++mul) {
            int carry = 0;
            for (int i = 0; i < len; ++i) {
                int prod = big[i] * 5 + carry;
                big[i] = prod % 10;
                carry   = prod / 10;
            }
            while (carry > 0) {
                big[len++] = carry % 10;
                carry      = carry / 10;
            }
        }

        /* Big‑endian array: most significant digit first */
        for (int i = 0; i < len; ++i)
            digits[i] = big[len - 1 - i];

        initialized = 1;
    }

    /* Map sequence index n (≥ -44) to array index 0…104 */
    int offset = -44;
    int idx    = (int)(n - offset);      /* n - (-44) = n + 44 */

    if (idx < 0 || idx >= 105)
        return 0;   /* out of range – behaviour not required */

    return (seq_t)digits[idx];
}
