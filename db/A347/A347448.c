/*
 * A347448  Number of integer partitions of n with alternating product > 1.
 * https://oeis.org/A347448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) – A119620(n)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Compute partition numbers up to n using the pentagonal number theorem.
   p[0] = 1, and for n > 0:
   p(n) = Σ_{k=1..∞} (-1)^{k-1} [p(n - k(3k-1)/2) + p(n - k(3k+1)/2)]
   Terminate when the pentagonal numbers exceed n.              */
static seq_t *partitions(idx_t limit)
{
    seq_t *p = mem_new(seq_t, (limit + 1));
    if (!p) return NULL;
    p[0] = 1;
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t pos = 0, neg = 0;               /* accumulate separately */
        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            seq_t term = p[i - pent1];
            if (k & 1) pos += term; else neg += term;
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= i) {
                term = p[i - pent2];
                if (k & 1) pos += term; else neg += term;
            }
        }
        p[i] = pos - neg;                      /* pos ≥ neg always */
    }
    return p;
}

seq_t a(idx_t n)
{
    /* Need partitions for n and floor(n/2)   */
    idx_t max_val = n;                         /* n is enough for both */
    seq_t *p = partitions(max_val);
    if (!p) return 0;                          /* allocation failed */

    seq_t result = p[n] - p[n / 2];            /* a(n) = p(n) – p(floor(n/2)) */
    mem_free(p);
    return result;
}
