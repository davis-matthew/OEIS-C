/*
 * A319617  Number of Integer solutions to w^2 + x^2 + y^2 + z^2 < n^2; number
 *          of lattice points inside a 4-sphere of radius n.
 * https://oeis.org/A319617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A319617: Number of integer solutions to w^2 + x^2 + y^2 + z^2 < n^2.
 * Using the identity
 *   r4(k) = 8 * Σ_{d|k, 4∤d} d   (for k > 0),  r4(0) = 1
 * the count is
 *   a(n) = 1 + 8 * Σ_{k=1}^{n^2-1} Σ_{d|k, 4∤d} d
 *       = 1 + 8 * ( Σ_{d=1}^{n^2-1} d * ⌊(n^2-1)/d⌋
 *           – Σ_{d≡0 (mod 4)} d * ⌊(n^2-1)/d⌋ ).
 * The summations are evaluated in O(√n) time by grouping equal quotients.
 */

#include <stdint.h>

static seq_t sum_step(idx_t step, idx_t N)
{
    /* Sum_{d = step, 2·step, … ≤ N} d * ⌊N/d⌋  */
    seq_t total = 0;
    idx_t limit = N / step;
    idx_t i = 1;
    while (i <= limit) {
        idx_t v = N / (step * i);
        /* Next i where floor(N/(step*i)) changes.  */
        idx_t j = N / (step * v) + 1;
        if (j > limit) j = limit + 1;
        idx_t cnt = j - i;
        seq_t sum_k = ((seq_t)i + (seq_t)(j - 1)) * (seq_t)cnt / 2;
        total += sum_k * (seq_t)step * (seq_t)v;
        i = j;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 0 -> a(0)=0 */
    if (n == 1) return 1;          /* fast‑path for n=1 */

    /* N = n^2 - 1, the largest k with k < n^2 */
    seq_t nsq = (seq_t)n * (seq_t)n;
    seq_t N = nsq - 1;

    /* Compute the two divisor sums */
    seq_t S_all = sum_step(1, (idx_t)N);       /* Σ d * ⌊N/d⌋   for all d */
    seq_t S_4   = sum_step(4, (idx_t)N);       /* Σ d * ⌊N/d⌋   for d ≡ 0 mod 4 */

    /* a(n) = 1 + 8 * (S_all - S_4) */
    return (seq_t)1 + (seq_t)8 * (S_all - S_4);
}
