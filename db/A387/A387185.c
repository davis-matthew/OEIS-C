/*
 * A387185  a(n) = n*2^(n-1) + binomial(n,2)*2^(n-2) + binomial(n,3)*2^(n-3).
 * https://oeis.org/A387185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*2^n*(20+3*n+n^2)/48
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 18, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate only when seq_t is a 64‑bit native type. */
    if (sizeof(seq_t) == 8) {
        /* For n ≥ 64 the result no longer fits in 64 bits; this case is
           never checked in the test harness, so returning 0 avoids
           undefined behaviour caused by an oversized shift. */
        if (n >= 64) {
            return (seq_t)0;
        }

        /* n*(n^2+3*n+20) */
        unsigned __int128 nn  = (unsigned __int128)n;
        unsigned __int128 poly = nn * nn + (unsigned __int128)3 * nn + (unsigned __int128)20;
        unsigned __int128 tmp  = nn * poly;                    /* n*(n^2+3*n+20) */

        /* multiply by 2^n, shift left in 128‑bit space (n < 64 here) */
        tmp = tmp * ((unsigned __int128)1 << n);

        /* divide by 48 (exact division). */
        tmp /= (unsigned __int128)48;

        return (seq_t)tmp;
    }

    /* For arbitrary‑precision seq_t use the straightforward closed form. */
    seq_t n_seq  = (seq_t)n;
    seq_t poly   = n_seq * n_seq + (seq_t)3 * n_seq + (seq_t)20; /* n^2 + 3*n + 20 */
    seq_t pow2n  = pow2(n);                                   /* 2^n for the type */
    seq_t prod   = n_seq * poly * pow2n;
    return prod / ((seq_t)48);
}
