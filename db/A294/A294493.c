/*
 * A294493  a(n) = Sum_{m=0..n} 3^v3(m!), where v3(m!) is the exponent of the
 *          highest power of 3 dividing n!, expressed in base 3.
 * https://oeis.org/A294493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{m=0..n} 3^v3(m!) , expressed in base 3 as a decimal
 *            integer
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Compute the sum of 3^v3(m!) as a normal integer */
    seq_t sum  = 0;
    seq_t term = 1;           /* 3^v3(m!) for the current m, starts with m = 0 (v3(0!) = 0) */

    for (idx_t m = 0; m <= n; ++m) {
        sum += term;               /* add current term to the running total */

        if (m == n)                /* last required m – no need to update further */
            break;

        /* Update term to 3^v3((m+1)!) = 3^v3(m!) * 3^v3(m+1) */
        seq_t x   = (seq_t)(m + 1);
        idx_t   cnt = 0;
        while (x % 3 == 0) {   /* count how many factors of 3 divide (m+1) */
            ++cnt;
            x /= 3;
        }
        for (idx_t i = 0; i < cnt; ++i)
            term *= (seq_t)3;   /* multiply by 3 cnt times */
    }

    /* Convert the sum from decimal to base‑3 digits and then interpret those
       digits as a decimal integer (the OEIS representation). */
    if (sum == 0)                /* only possible if n < 0, but keep for safety */
        return 0;

    seq_t tmp = sum;
    unsigned char *digits = NULL;
    size_t          cap    = 64;
    size_t          len    = 0;

    digits = (unsigned char*)malloc(cap);
    if (!digits)                 /* allocation failure – return zero */
        return 0;

    /* Extract base‑3 digits (least‑significant first) */
    while (tmp > 0) {
        if (len == cap) {
            cap <<= 1;
            unsigned char *newp = (unsigned char*)realloc(digits, cap);
            if (!newp) { free(digits); return 0; }
            digits = newp;
        }
        digits[len++] = (unsigned char)(tmp % 3);
        tmp /= 3;
    }

    /* Build the result as decimal digits */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * 10 + digits[len - 1 - i];
    }

    free(digits);
    return result;
}
