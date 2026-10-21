/*
 * A283531  Number of steps to return to n through a chain-addition sequence
 *          mod 10 with window of size equal to the number of digits of n.
 * https://oeis.org/A283531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the number of steps in the chain‑addition * sequence
 *            that returns to n, where each step deletes the * most
 *            significant digit of the current number, appends the * sum of
 *            the digits of the current number modulo 10 as the * new least
 *            significant digit, and the window size is the * number of...
 * Range:     u8 n <= 69, u16 n <= 9461, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, digit_sum, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* Initial value and window length (in digits) */
    seq_t orig  = (seq_t)n;
    seq_t cur   = orig;
    int   len0  = digit_len(n);
    seq_t mod   = pow10((idx_t)(len0 - 1));   /* 10^(len0-1) */

    seq_t steps = 0;
    do
    {
        seq_t low = cur % mod;                         /* drop MSD */
        seq_t s   = digit_sum(cur) % (seq_t)10;         /* new LSD */
        cur = low * 10 + s;                             /* next value */
        ++steps;
    }
    while (cur != orig);

    return steps;
}
