/*
 * A380585  a(n) = floor(n^2 / 10^m) + (n^2 mod 10^m) where m is the number of
 *          decimal digits in n.
 * https://oeis.org/A380585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = floor(n^2 / 10^m) + (n^2 mod 10^m), where m =
 *            digit_len(n)
 * Range:     u8 n <= 15, u16 n <= 323, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;                     /* n^2 */
    int m   = digit_len(nn);                /* number of decimal digits in n */
    seq_t p   = pow10((idx_t)m);            /* 10^m */
    seq_t high = sq / p;                    /* floor(n^2 / 10^m) */
    seq_t low  = sq % p;                    /* n^2 mod 10^m */
    return high + low;
}
