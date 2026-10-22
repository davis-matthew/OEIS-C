/*
 * A319853  Number of ways to write n as the sum of 4 positive integers
 *          a,b,c,d such that a/b - c/d = (a - c)/(b + d).
 * https://oeis.org/A319853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count solutions of a/b - c/d = (a - c)/(b + d) with a+b+c+d = n,
 *            a,b,c,d positive integers
 * Range:     u8 n <= 173, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The smallest possible sum is 1+1+1+1 = 4. */
    if (n < 4) return 0;

    seq_t count = 0;
    seq_t nseq  = n;          /* cast to seq_t for arithmetic */

    /* Enumerate coprime pairs (x, y) with x, y > 0 and x² ≤ n, y² ≤ n. */
    for (idx_t x = 1; (seq_t)x * (seq_t)x <= nseq; ++x) {
        for (idx_t y = 1; (seq_t)y * (seq_t)y <= nseq; ++y) {
            if (gcd_idx(x, y) != 1) continue;

            seq_t A = (seq_t)x * (seq_t)x + (seq_t)y * (seq_t)y;  /* x² + y² */
            seq_t B = (seq_t)x + (seq_t)y;                         /* x + y */

            /* Minimum possible left‑hand side with k = g = 1 must fit */
            if (A + B > nseq) continue;

            /* Maximum g such that remainder n - B*g ≥ A */
            seq_t gmax_seq = (nseq - A) / B;
            if (gmax_seq < 1) continue;
            idx_t gmax = (idx_t)gmax_seq;

            for (idx_t g = 1; g <= gmax; ++g) {
                seq_t rem = nseq - B * (seq_t)g;
                /* rem must be a multiple of A to give integer k ≥ 1 */
                if (rem % A == 0) {
                    /* k = rem / A automatically ≥ 1 */
                    count += 1;
                }
            }
        }
    }
    return count;
}
