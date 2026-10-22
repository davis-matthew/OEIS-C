/*
 * A332554  a(n) = (n*(n+1)/2)/Q(n) - (Q(n)+1)/2, where Q(n) = A332547(n).
 * https://oeis.org/A332554
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = min_{Q odd divisor of T(n)} [ T(n)/Q - (Q+1)/2 ],
 * where T(n) = n*(n+1)/2 (triangular number).
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute a(n) for n >= 2 */
seq_t a(idx_t n)
{
    /* triangular number T = n*(n+1)/2 */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;
    idx_t Tidx = (idx_t)T;                 /* safe for n ≤ 10000 */

    seq_t best = 0;
    bool found = false;

    /* iterate over divisors up to sqrt(T) */
    for (idx_t d = 1; d * d <= Tidx; ++d) {
        if (T % (seq_t)d != 0) continue;  /* d is not a divisor */

        /* First divisor d (if odd) */
        if (d & 1) {
            seq_t val = T / (seq_t)d - (seq_t)((d + 1) / 2);
            if (val > 0) {
                if (!found || val < best) {
                    best = val;
                    found = true;
                }
            }
        }

        /* Paired divisor q = T/d (if different from d) */
        idx_t q = Tidx / d;
        if (q != d && (q & 1)) {
            seq_t val = T / (seq_t)q - (seq_t)((q + 1) / 2);
            if (val > 0) {
                if (!found || val < best) {
                    best = val;
                    found = true;
                }
            }
        }
    }

    return best;
}
