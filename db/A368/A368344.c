/*
 * A368344  a(n) = Sum_{k=0..n} 3^(n-k) * floor(k/3).
 * https://oeis.org/A368344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor(3^(n+1)/26) - floor((n+1)/3))/2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For small integer seq_t, compute by recurrence to avoid overflow.
   For large / arbitrary‑precision seq_t, use the closed form. */
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0ULL || n == 1ULL || n == 2ULL)
        return (seq_t)0;

    /* Small (builtin unsigned) seq_t: use recurrence with modulo arithmetic. */
    if (sizeof(seq_t) <= 8) {
        const unsigned k = (unsigned)(sizeof(seq_t) * 8);          /* number of bits */
        const unsigned __int128 mask = (((unsigned __int128)1 << (k + 1)) - 1);
        unsigned __int128 power = 3;  /* 3^(i-2) for i = 3 */

        seq_t a0 = 0;  /* a(i-3) */
        seq_t a1 = 0;  /* a(i-2) */
        seq_t a2 = 0;  /* a(i-1) */

        for (idx_t i = 3; i <= n; ++i) {
            /* increment term (3^(i-2)-1)/2 */
            seq_t inc = (seq_t)((power - 1) >> 1);

            seq_t next = a0 + inc;  /* a(i) */

            /* shift the window */
            a0 = a1;
            a1 = a2;
            a2 = next;

            /* update power modulo 2^(k+1) */
            power = (power * 3) & mask;
        }
        return a2;
    }

    /* Large / arbitrary‑precision seq_t: use the closed‑form formula. */
    seq_t power = pow3(n + 1);                 /* 3^(n+1) */
    seq_t div26 = power / (seq_t)26;           /* floor(3^(n+1)/26) */
    seq_t div3  = (seq_t)((n + 1) / 3);        /* floor((n+1)/3) */
    return (div26 - div3) / (seq_t)2;          /* final result */
}
