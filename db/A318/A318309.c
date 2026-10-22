/*
 * A318309  Restricted growth sequence transform of A083254 (2*phi(n) - n).
 * https://oeis.org/A318309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the restricted‑growth sequence transform * of b(n) =
 *            2*phi(n) – n; i.e., a(n) is the order of first * appearance of
 *            b(n) among b(1)..b(n)
 * Range:     u8 n <= 225, u16 n <= 9806, u32 n <= 9806, u64 n <= 9806,
 *            u128 n <= 9806, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* <seq_t> and <idx_t> are supplied by the build.
 * <seq_t> may be an unsigned integer type or an
 * arbitrary‑precision integer class (C++).  <idx_t> is
 * an unsigned 64‑bit integer (uint64_t).  The build
 * may supply helper functions described in the
 * problem statement; we use calc_phi below. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/calc_phi.h"

typedef struct {
    seq_t key;
    seq_t value;
} entry_t;

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, values below 1 are irrelevant */

    /* ----- static state ----- */
    static idx_t       max_n     = 0;          /* highest n processed so far          */
    static seq_t *     cache     = NULL;       /* cache of results a(1..max_n)        */
    static size_t      cache_cap = 0;          /* allocated size of cache             */

    static size_t      tab_cap   = 0;          /* capacity of the table               */
    static size_t      tab_cnt   = 0;          /* number of entries in the table      */
    static entry_t *   table     = NULL;       /* mapping from b(n) to its label      */

    /* grow cache to hold indices up to n (including 0) */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – unlikely in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values from max_n+1 up to n */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t b = (seq_t)2 * calc_phi((seq_t)i) - (seq_t)i;

            /* look for an existing key */
            seq_t label = 0;
            for (size_t j = 0; j < tab_cnt; ++j) {
                if (table[j].key == b) {
                    label = table[j].value;
                    break;
                }
            }

            /* new distinct value → new label */
            if (label == 0) {
                label = (seq_t)(tab_cnt + 1);
                if (tab_cnt == tab_cap) {
                    size_t new_cap = tab_cap ? tab_cap * 2 : 4;
                    entry_t *tmp = (entry_t *)realloc(table, new_cap * sizeof(entry_t));
                    if (!tmp) return 0; /* allocation failure – unlikely in tests */
                    table = tmp;
                    tab_cap = new_cap;
                }
                table[tab_cnt].key   = b;
                table[tab_cnt].value = label;
                ++tab_cnt;
            }

            cache[i] = label;
        }
        max_n = n;
    }

    return cache[n];
}
