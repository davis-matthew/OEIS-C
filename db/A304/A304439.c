/*
 * A304439  Add to n the sum of its odd digits minus the sum of its even
 *          digits.
 * https://oeis.org/A304439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + (sum of odd digits of n) - (sum of even digits of n)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;     /* start with n itself */
    long long diffSum = 0;    /* sum_{odd} minus sum_{even} */

    idx_t temp = n;
    while (temp > 0) {
        unsigned digit = (unsigned)(temp % 10); /* digit 0‑9 */
        if (digit & 1)      /* odd digit */
            diffSum += digit;
        else                /* even digit */
            diffSum -= digit;
        temp /= 10;
    }

    if (diffSum > 0)
        res += (seq_t)diffSum;
    else if (diffSum < 0)
        res -= (seq_t)(-diffSum);

    return res;
}
