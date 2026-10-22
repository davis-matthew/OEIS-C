/*
 * A382269  The factorial base expansion of a(n) is, when read from right to
 *          left, the ordinal transform of that of n.
 * https://oeis.org/A382269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the factorial‑base expansion of a(n) is the ordinal
 *            transform of the factorial‑base digits of n (read from right to
 *            left). That is, for each factorial‑base digit v at position i (i
 *            = 0 for 1!, i = 1 for 2!, …), the i‑th digit of a(n) is the
 *            (count of v among the first i+1 digits) in the ba...
 * Range:     u8 n <= 120, u16 n <= 5039, u32 n <= 5039, u64 n <= 5039,
 *            u128 n <= 5039, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset n = 0 */
    if (n == 0) return (seq_t)0;

    /* Step 1: compute the factorial‑base digits of n (least
       significant first).  For n < 2^64, at most 21 digits are needed. */
    unsigned char digits[64];
    size_t k = 0;           /* number of digits */
    idx_t t   = n;
    size_t i  = 1;          /* divisor increment: 2,3,4,... for 1!,2!,3!,... */

    while (t > 0) {
        digits[k++] = (unsigned char)(t % (i + 1));
        t /= (i + 1);
        ++i;
    }

    /* Step 2: compute the ordinal transform (count of each digit
       up to its position). */
    unsigned char e[64]   = {0};
    unsigned char count[64] = {0};

    for (size_t j = 0; j < k; ++j) {
        unsigned char v = digits[j];
        ++count[v];
        e[j] = count[v];
    }

    /* Step 3: convert the transformed digits back to a number
       (using the factorial weights). */
    seq_t  result = 0;
    seq_t  fact   = 1;       /* 1! = 1 */

    for (size_t j = 0; j < k; ++j) {
        result += (seq_t)e[j] * fact;
        fact   *= (seq_t)(j + 2);   /* next factorial weight ( (j+2)! / (j+1)! ) */
    }

    return result;
}
