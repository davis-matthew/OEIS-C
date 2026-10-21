/*
 * A246293  Numbers k such that sin(k) > sin(k+1).
 * https://oeis.org/A246293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer k such that sin(k) > sin(k+1) Using sin
 *            k – sin (k+1) = –2 sin(½) cos(k+½), the inequality holds ⇔
 *            cos(k+½) < 0. Let θ = 1/π. Then cos(k+½) < 0 ⇔ let N = (2k+1)·θ,
 *            let f = {N} (fractional part of N), let p = ⌊N⌋ (mod 2). The
 *            condition is (f > ½) xor (p == 1). The code impl...
 * Range:     u8 n <= 129, u16 n <= 4000, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* High‑precision rational approximation of 1/π */
    static const unsigned __int128 P = (unsigned __int128)318309886183790672ULL;
    static const unsigned __int128 Q = (unsigned __int128)1000000000000000000ULL;
    static const unsigned __int128 TWOQ = Q * 2;

    idx_t m = 0;          /* candidate integer k */
    idx_t count = 0;      /* how many members found so far */

    while (count < n)
    {
        /* N = (2*m + 1) * P  (128‑bit to avoid overflow) */
        unsigned __int128 N = ((unsigned __int128)(2 * (unsigned __int128)m + 1)) * P;

        /* r = N mod 2Q  (fractional part × 2Q) */
        unsigned __int128 r = N % TWOQ;

        /* k = floor(N / 2Q)  (used for parity) */
        unsigned __int128 k = N / TWOQ;

        /* Condition: (r > Q) xor (k % 2 == 1) */
        if ( ((r > Q) ^ ((k & 1) != 0)) )
        {
            ++count;
            if (count == n)
                return (seq_t)m;   /* nth element found */
        }
        ++m;
    }

    return (seq_t)0;  /* should never reach here */
}
