/*
 * A357681  Square array T(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. cosh( sqrt(k) * (exp(x) - 1)
 *          ).
 * https://oeis.org/A357681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{k=0..floor(i/2)} j^k * S(i,2k), where S(.,.) is the
 *            Stirling number of the second kind. The indices i and j are
 *            obtained from the antidiagonal coordinates of a(n): let s be the
 *            least integer such that s*(s+1)/2 > n, then i = n - s*(s+1)/2
 *            and j = s - i
 * Range:     u8 n <= 41, u16 n <= 86, u32 n <= 167, u64 n <= 400,
 *            u128 n <= 1028, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#ifndef MAX_STIRLING_N
#define MAX_STIRLING_N 140   /* enough for n up to 10000 */
#endif

static seq_t S[MAX_STIRLING_N + 1][MAX_STIRLING_N + 1];
static bool S_ready = false;

/* Pre‑compute Stirling numbers of the second kind up to MAX_STIRLING_N. */
static void init_stirling(void)
{
    if (S_ready)
        return;

    for (idx_t n = 0; n <= MAX_STIRLING_N; ++n) {
        for (idx_t m = 0; m <= MAX_STIRLING_N; ++m) {
            S[n][m] = 0;
        }
    }

    S[0][0] = 1;
    for (idx_t n = 1; n <= MAX_STIRLING_N; ++n) {
        S[n][0] = 0;
        for (idx_t m = 1; m <= n; ++m) {
            seq_t term1 = S[n - 1][m] * (seq_t)m;
            seq_t term2 = S[n - 1][m - 1];
            S[n][m] = term1 + term2;
        }
    }
    S_ready = true;
}

seq_t a(idx_t n)
{
    init_stirling();

    /* Find s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    /* Antidiagonal coordinates (i,j). */
    idx_t i = n - s * (s + 1) / 2;
    idx_t j = s - i;

    /* Compute T(i,j). */
    seq_t sum   = 0;
    seq_t power = 1;              /* j^k, starts with k = 0 */

    for (idx_t k = 0; 2 * k <= i; ++k) {
        seq_t term = S[i][2 * k] * power;
        sum += term;
        power *= (seq_t)j;
    }
    return sum;
}
