/*
 * A379603  a(n) is the largest n-digit number whose square contains only
 *          digits greater than 5.
 * https://oeis.org/A379603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10^n - (5*10^(n/2)+1)/3 if n is even; a(n) = 10*a(n-1) +
 *            6 if n is odd and n>=5; For n ≤ 9 a(n) is obtained by
 *            brute‐force search (the first 9 terms are 3, 83, 937, 9833,
 *            98336, 998333, 9994833, 99983333, 999939437)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"

static bool digits_ok(seq_t sq)
{
    while (sq > 0)
    {
        seq_t d = sq % 10;
        if (d <= 5)
            return false;
        sq /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Small n: brute‑force search */
    if (n <= 9)
    {
        seq_t limit = seq_pow(10, n);
        seq_t cur   = limit - 1;
        for (;;)
        {
            seq_t sq = cur * cur;
            if (digits_ok(sq))
                return cur;
            cur -= 1;
        }
    }

    /* Even n: closed form */
    if ((n & 1) == 0)
    {
        seq_t pow10_n     = seq_pow(10, n);
        seq_t pow10_half  = seq_pow(10, n / 2);
        seq_t sub         = (5 * pow10_half + 1) / 3;
        return pow10_n - sub;
    }

    /* Odd n ≥ 11: use the recurrence a(2k+1)=10*a(2k)+6 */
    seq_t pow10_prev   = seq_pow(10, n - 1);    /* 10^(n−1)  */
    seq_t pow10_half   = seq_pow(10, (n - 1) / 2); /* 10^((n−1)/2) */
    seq_t sub          = (5 * pow10_half + 1) / 3;
    seq_t even_term    = pow10_prev - sub;     /* a(n−1)        */
    return even_term * 10 + 6;
}
