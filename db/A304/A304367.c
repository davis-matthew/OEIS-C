/*
 * A304367  Numbers n with additive persistence = 2.
 * https://oeis.org/A304367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A304367: Numbers n with additive persistence = 2
   Computed by scanning integers and testing digit sums. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_digits.h"

/* sum of decimal digits of x */
static seq_t digit_sum(seq_t x)
{
    uint8_t buf[64];
    size_t len;
    get_digits(x, buf, &len);
    seq_t s = 0;
    for (size_t i = 0; i < len; ++i)
        s += buf[i];
    return s;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* safety for index 0 */

    seq_t cur = 1;                    /* next candidate to test */
    idx_t count = 0;                  /* how many persistence‑2 numbers found */

    while (count < n) {
        seq_t s1 = digit_sum(cur);
        if (s1 >= 10) {               /* need at least two steps */
            seq_t s2 = digit_sum(s1);
            if (s2 <= 9)              /* final step produces one digit */
                ++count;
        }
        cur = cur + 1;                 /* move to next integer */
    }
    return cur - 1;                    /* last number that satisfied the test */
}
