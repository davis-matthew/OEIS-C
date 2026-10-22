/*
 * A354463  a(n) is the number of trailing zeros in (2^n)!.
 * https://oeis.org/A354463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (2^n - s_5(2^n)) / 4 = Σ_{k≥1} ⌊2^n / 5^k⌋
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 129,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of trailing zeros in (2^n)! */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 32/64‑bit or small integer types: use 128‑bit arithmetic for n < 128 */
    if (sizeof(seq_t) <= 8 && n < 128) {
        /* Build 2^n as an unsigned __int128 (safe up to n < 128) */
        unsigned __int128 big = ((unsigned __int128)1) << n;
        unsigned __int128 pow5 = 5;
        unsigned __int128 sum  = 0;

        /* Sum ⌊big / 5^k⌋ until the power exceeds big */
        while (pow5 <= big && pow5 > 0) {
            sum += big / pow5;
            pow5 *= 5;
        }
        /* For n where the result fits in seq_t this cast is exact */
        return (seq_t)sum;
    }

    /* 128‑bit or arbitrary‑precision types: use big‑integer operations */
    seq_t N = pow2(n);        /* N = 2^n in the given seq_t type */
    seq_t sum5 = 0;
    seq_t tmp = N;
    seq_t five = 5;

    /* Extract base‑5 digits of N and sum them */
    while (tmp > 0) {
        seq_t digit = tmp % five;   /* digit < 5 */
        sum5 += digit;
        tmp /= five;
    }

    /* a(n) = (N - sum5) / 4  */
    return (N - sum5) / 4;
}
