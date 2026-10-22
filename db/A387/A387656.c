/*
 * A387656  Array read by ascending antidiagonals: A(n, k) = numerator(k^n/n),
 *          with k >= 0.
 * https://oeis.org/A387656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, tabl, changed
 * Formula:   a(n) = numerator(K^N / N), where N and K are obtained by reading
 *            the array defined by A(N, K) = numerator(K^N / N) along
 *            ascending antidiagonals
 * Range:     u8 n <= 32, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal index s such that s*(s+1)/2 >= n */
    idx_t s = 0;
    while (s * (s + 1) / 2 < n)
        ++s;

    /* Elements preceding this diagonal */
    idx_t prev_tri = (s - 1) * s / 2;
    /* Position (0‑based) within the diagonal */
    idx_t pos = n - prev_tri - 1;

    /* Corresponding row (N) and column (K) */
    idx_t N = s - pos;
    idx_t K = pos;

    /* Compute K^N and reduce with N */
    seq_t power = big_pow((seq_t)K, (seq_t)N);
    seq_t g     = gcd(power, (seq_t)N);
    return power / g;
}
