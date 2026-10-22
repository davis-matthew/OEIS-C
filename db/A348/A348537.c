/*
 * A348537  Number of partitions of n into 3 parts whose largest part divides
 *          n.
 * https://oeis.org/A348537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1019, u16 n <= 9644, u32 n <= 9644, u64 n <= 9644,
 *            u128 n <= 9644, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A348537: number of partitions of n into 3 parts whose largest part divides n.
   Formula: a(n) = ∑_{d|n, d≤n‑2} max(0, min(d, n‑d‑1) – ceil((n‑d)/2) + 1) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    seq_t total = (seq_t)0;
    for (idx_t i = (idx_t)1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* first divisor */
            idx_t d = i;
            if (d <= n - 2) {
                idx_t S = n - d; /* sum of the other two parts */
                if (S >= 2) {
                    idx_t ceilS = (S + 1) / 2;         /* ceil(S/2) */
                    idx_t limit = (d < (S - 1)) ? d : (S - 1); /* min(d, S-1) */
                    if (limit >= ceilS) {
                        idx_t cnt = limit - ceilS + 1;
                        total += (seq_t)cnt;
                    }
                }
            }

            /* paired divisor n/i, avoid double counting when i*i == n */
            idx_t d2 = n / i;
            if (d2 != i) {
                if (d2 <= n - 2) {
                    idx_t S = n - d2;
                    if (S >= 2) {
                        idx_t ceilS = (S + 1) / 2;
                        idx_t limit = (d2 < (S - 1)) ? d2 : (S - 1);
                        if (limit >= ceilS) {
                            idx_t cnt = limit - ceilS + 1;
                            total += (seq_t)cnt;
                        }
                    }
                }
            }
        }
    }
    return total;
}
