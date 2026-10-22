/*
 * A380121  a(n) = C(n, Q(n+3, 4)-1)*C(n, Q(n+1, 4)) + C(n, Q(3*n+1, 4))*C(n,
 *          Q(3*n+3, 4)) where C = binomial and Q(x, y) = floor(x/y).
 * https://oeis.org/A380121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n, Q(n+3,4)-1)*C(n, Q(n+1,4)) + C(n, Q(3*n+1,4))*C(n,
 *            Q(3*n+3,4))
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Return C(n,k) if 0 ≤ k ≤ n, otherwise 0. */
static seq_t binom_safe(idx_t n, idx_t k)
{
    if (k > n) return 0;
    return binomial(n, k);
}

seq_t a(idx_t n)
{
    /* Q(n+3,4) and Q(n+1,4) */
    idx_t q1 = (n + 3) / 4;
    idx_t q2 = (n + 1) / 4;

    /* Compute Q(3*n+1,4) and Q(3*n+3,4) safely */
    unsigned __int128 three_n = (unsigned __int128)3 * n;
    idx_t q3 = (three_n + 1) / 4;
    idx_t q4 = (three_n + 3) / 4;

    /* First product: C(n, q1-1)*C(n, q2) */
    seq_t p1a = (q1 == 0) ? 0 : binom_safe(n, q1 - 1);
    seq_t p1b = binom_safe(n, q2);
    seq_t prod1 = p1a * p1b;

    /* Second product: C(n, q3)*C(n, q4) */
    seq_t p2a = binom_safe(n, q3);
    seq_t p2b = binom_safe(n, q4);
    seq_t prod2 = p2a * p2b;

    return prod1 + prod2;
}
