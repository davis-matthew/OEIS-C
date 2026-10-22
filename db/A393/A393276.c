/*
 * A393276  a(n) = Sum_{k=0..floor(n/4)} binomial(2*k,k) *
 *          binomial(n-k-1,n-4*k).
 * https://oeis.org/A393276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/4⌋} binomial(2k,k) * binomial(n‑k‑1, n‑4k)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 31, u64 n <= 60, u128 n <= 116,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(0) is a special case because n‑k‑1 would be ‑1 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    const idx_t maxk = n / 4;   /* floor(n/4) */

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binom((idx_t)(2 * k), k);              /* binomial(2k,k)   */
        idx_t nk1 = n - k - 1;                            /* n‑k‑1            */
        idx_t r   = n - 4 * k;                            /* n‑4k             */
        seq_t t2 = binom(nk1, r);                         /* binomial(n‑k‑1,n‑4k) */
        sum += t1 * t2;
    }
    return sum;
}
