/*
 * A243212  Number of ways to place 3 points on a triangular grid of side n so
 *          that no three of them are vertices of an equilateral triangle with
 *          sides parallel to the grid.
 * https://oeis.org/A243212
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n*(n+1)/2, 3) – floor((n-1)*(n+1)*(2*n-1)/8) (OEIS
 *            A243212)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 76, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 49 terms through n = 986 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t choose3(seq_t N) {
    /* compute N*(N-1)*(N-2)/6 avoiding overflow for 64‑bit seq_t by canceling factors 2 and 3 */
    seq_t a = N, b = N - 1, c = N - 2;

    /* divide by 2 */
    if (a % 2 == 0)      a /= 2;
    else if (b % 2 == 0) b /= 2;
    else                 c /= 2;

    /* divide by 3 */
    if (a % 3 == 0)      a /= 3;
    else if (b % 3 == 0) b /= 3;
    else                 c /= 3;

    return a * b * c;
}

static inline seq_t sub_term(idx_t n) {
    /* floor((n-1)*(n+1)*(2*n-1)/8) using a 128‑bit intermediate to avoid overflow   */
    unsigned __int128 t1 = (unsigned __int128)(n - 1);
    unsigned __int128 t2 = (unsigned __int128)(n + 1);
    unsigned __int128 t3 = (unsigned __int128)(2 * n - 1);
    unsigned __int128 prod = t1 * t2 * t3;
    unsigned __int128 div = prod / 8;
    return (seq_t)div;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;                /* outside the official offset – undefined */

    /* N = n*(n+1)/2  (computed with early division to avoid overflow) */
    seq_t sn = (seq_t)n;
    seq_t N;
    if (sn % 2 == 0)
        N = (sn / 2) * (sn + 1);
    else
        N = sn * ((sn + 1) / 2);

    seq_t comb = choose3(N);
    seq_t sub  = sub_term(n);
    return comb - sub;
}
