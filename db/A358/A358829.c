/*
 * A358829  Number of twice-partitions of n with no (1)'s.
 * https://oeis.org/A358829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of \prod_{k>=2} 1/(1 - p(k) x^k) in the
 *            ordinary generating function, where p(k) is the number of
 *            partitions of k
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper: free an array allocated by alloc_seq. */
static void free_seq(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }

    size_t N = (size_t)n;            /* storage bounds fit in size_t       */
    seq_t *P = alloc_seq_array(N + 1);   /* p(k) for k = 0..n                */
    seq_t *F = alloc_seq_array(N + 1);   /* a values for 0..n                */

    if (!P || !F) {                  /* allocation failure */
        return 0;
    }

    /* compute partition numbers p(k) for 0 <= k <= n   */
    P[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; ++j) {
            P[j] += P[j - i];
        }
    }

    /* compute a(n) from the generating function   */
    F[0] = 1;
    for (idx_t k = 2; k <= n; ++k) {
        seq_t pk = P[k];
        for (idx_t j = k; j <= n; ++j) {
            F[j] += F[j - k] * pk;
        }
    }

    seq_t result = F[n];
    free_seq(P);
    free_seq(F);
    return result;
}
