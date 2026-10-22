/*
 * A380471  G.f. A(x) satisfies A(x) = 1/( 1 - 25*x*A(x)^4 )^(1/5).
 * https://oeis.org/A380471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25^n * binomial((9*n+1)/5, n) / (9*n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 5, a(2) = 175, ... (as listed above) */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t nine_n = 9 * n;

    /* For 64‑bit (or smaller) seq_t we use a __int128 temporary to avoid
       overflow of intermediate values for n up to 11 (the largest n that
       fits in 64 bits).  For larger seq_t we rely on seq_t's built‑in
       arbitrary precision arithmetic. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 P = 1;               /* product of (9n+6-5k)   */
        unsigned __int128 fact = 1;            /* n!                   */
        unsigned __int128 five_n = 1;          /* 5^n                   */
        unsigned __int128 denom = 1;           /* n! * (9n+1)           */

        for (idx_t k = 1; k <= n; ++k) {
            P *= (unsigned __int128)(nine_n + 6 - 5 * k);
        }
        for (idx_t i = 2; i <= n; ++i) {
            fact *= (unsigned __int128)i;
        }
        for (int i = 0; i < n; ++i) {
            five_n *= 5;
        }
        denom = fact * (unsigned __int128)(nine_n + 1);

        unsigned __int128 num = P * five_n;
        unsigned __int128 res = num / denom;

        return (seq_t)res;
    } else {
        /* arbitrary‑precision path */
        seq_t P = 1;           /* product of (9n+6-5k)   */
        seq_t fact = 1;        /* n!                   */
        seq_t five_n = 1;      /* 5^n                   */

        for (idx_t k = 1; k <= n; ++k) {
            seq_t term = (seq_t)(nine_n + 6 - 5 * k);
            P *= term;
        }
        for (idx_t i = 2; i <= n; ++i) {
            fact *= (seq_t)i;
        }
        for (int i = 0; i < n; ++i) {
            five_n *= (seq_t)5;
        }

        seq_t denom = fact * (seq_t)(nine_n + 1);
        seq_t num = P * five_n;
        return num / denom;
    }
}
