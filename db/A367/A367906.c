/*
 * A367906  Numbers k such that it is possible to choose a different binary
 *          index of each binary index of k.
 * https://oeis.org/A367906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 121, u16 n <= 3541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_bii, kuhn_dfs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: brute‑force search for BII numbers using bipartite matching */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_bii.h"
#include "../utils/kuhn_dfs.h"

/* ##### Main sequence function ##### */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated capacity */
    static size_t count = 0;     /* number of stored terms   */
    static idx_t last_k_checked = 0; /* largest k already tested */

    /* Produce terms until we have at least n of them. */
    size_t target = (size_t)n;
    while (count < target) {
        idx_t k = ++last_k_checked;

        if (is_bii(k)) {
            if (count == cap) {
                size_t new_cap = (cap == 0) ? 8 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;
                cache = tmp;
                cap = new_cap;
            }
            cache[count++] = (seq_t)k;
        }
    }
    return cache[n - 1];
}
