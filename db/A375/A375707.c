/*
 * A375707  First differences minus 1 of nonsquarefree numbers.
 * https://oeis.org/A375707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (b[n+1] - b[n]) - 1 where b[n] is the n‑th nonsquarefree
 *            positive integer (offset 1). The value at n = 1 is 3 (4 is the
 *            first nonsquarefree, 0 is taken as the virtual predecessor).
 *            This implementation memoises all generated nonsquarefree numbers
 *            so that calls can arrive in any order. It us...
 * Range:     u8 n <= 613, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

static seq_t *nsf = NULL;        /* cache of nonsquarefree numbers (0‑based) */
static size_t nsf_len = 0;       /* number of cached elements                */
static idx_t next_candidate = 2; /* next integer to test for nonsquarefreeness */

seq_t a(idx_t n) {
    /* Ensure we have at least n+1 elements in the cache: nsf[0..n] */
    while (nsf_len <= n) {
        for (idx_t m = next_candidate; ; ++m) {
            if (!is_squarefree((seq_t)m)) {
                /* Store the new nonsquarefree number */
                seq_t *tmp = (seq_t *)realloc(nsf, (nsf_len + 1) * sizeof(seq_t));
                if (!tmp) {
                    /* Out of memory – return 0 as a safe fallback */
                    if (nsf) free(nsf);
                    nsf = NULL;
                    nsf_len = 0;
                    return (seq_t)0;
                }
                nsf = tmp;
                nsf[nsf_len++] = (seq_t)m;
                next_candidate = m + 1;
                if (nsf_len > n) break;
            }
        }
    }

    /* a(1) uses nsf[1] - nsf[0] - 1; in general a(n) = nsf[n] - nsf[n-1] - 1 */
    seq_t diff = nsf[n] - nsf[n - 1];
    return diff - (seq_t)1;
}
