/*
 * A337770  a(0)=1; a(n) is the leading digit of a(n-1) multiplied by n
 *          concatenated with the remaining digits of a(n-1).
 * https://oeis.org/A337770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1;
   For n > 0: a(n) = concat( (first digit of a(n‑1) * n) , rest of a(n‑1) ).
   The “concat” operation is performed by multiplying the leading part by
   10^(k) where k = digit count of a(n‑1) – 1, and adding the remaining
   digits. */
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)1;

    seq_t curr = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        int len  = digit_len(curr);                     /* number of decimal digits of curr */
        seq_t base = pow10((idx_t)(len - 1));           /* 10^(len-1) */
        seq_t leading = curr / base;                    /* leading digit of curr */
        seq_t rest     = curr % base;                   /* remaining digits of curr */
        seq_t mul      = leading * (seq_t)i;            /* leading digit multiplied by i */
        curr = mul * base + rest;                      /* concatenate mul and rest */
    }
    return curr;
}
