/*
 * A292930  Triangle read by rows: T(n,k) (n>=1, 3<=k<=n+2) is the number of
 *          k-sequences of balls colored with at most n colors such that
 *          exactly three balls are the same color as some other ball in the
 *          sequence.
 * https://oeis.org/A292930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(N,K), where N = smallest integer with N(N+1)/2 ≥ n, K =
 *            2 + (n – N(N–1)/2) and T(N,K) = binom(K,3) * N!/(N+2–K)!. This
 *            matches OEIS A292930
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* n must be ≥ 1; for n<1 we simply return 0. */
    if (n < 1) return 0;

    /* Find triangle row N: smallest N with N(N+1)/2 ≥ n. */
    idx_t low = 1, high = 1;
    /* Expand high until it is large enough, using 128‑bit to avoid overflow. */
    while (( (__uint128_t)high * (high + 1) / 2 ) < n) {
        if (high > (UINT64_MAX >> 1)) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;          /* double high safely */
    }
    /* Binary search [low,high] for the correct N. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( (__uint128_t)mid * (mid + 1) / 2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    idx_t N = low;          /* row index (1‑based) */

    /* Position of the element within row N (1‑based). */
    idx_t before = (idx_t)( ( (__uint128_t)(N - 1) * N ) / 2 );   /* sum of previous rows */
    idx_t pos = n - before;                                         /* 1 ≤ pos ≤ N */

    /* Compute K = 2 + pos and binomial(K,3). */
    seq_t K = (seq_t)(pos + 2);            /* K fits in seq_t comfortably */
    seq_t binom = K * (K - 1) * (K - 2) / 6;   /* binom(K,3) */

    /* Compute the falling factorial N!/(N‑pos)! = N · (N‑1) · … · (N‑pos+1). */
    seq_t falling = 1;
    for (idx_t i = 0; i < pos; ++i)
        falling *= (seq_t)(N - i);

    /* Result = binom(K,3) * falling_factorial. */
    return binom * falling;
}
