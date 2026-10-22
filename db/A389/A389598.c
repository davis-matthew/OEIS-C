/*
 * A389598  Standard composition numbers of compositions without distinct
 *          first differences.
 * https://oeis.org/A389598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k whose standard A066099 composition has
 *            * non-distinct first differences. The function enumerates k and
 *            selects those * with repeated differences in the composition
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------  Standard includes  ---------- */
#include <stdlib.h>
#include <stdbool.h>

/* ----------  Main function  ---------- */
seq_t a(idx_t n) {
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;      /* array of a(1)..a(current_count)       */
    static idx_t  cache_count = 0;   /* how many terms are stored (a(1) is idx 0)  */
    static idx_t  next_k       = 0;   /* next k to test                               */

    /* Expand cache if necessary */
    if (n > cache_count) {
        /* Reallocate cache to accommodate at least n terms */
        seq_t *new_cache = NULL;
        new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failure */
        cache = new_cache;

        /* Enumerate k until we have n terms */
        while (cache_count < n) {
            /* Test current k for repeated first differences */
            bool has_dup = false;
            {
                /* Compute composition parts for k */
                seq_t tmp  = (seq_t)next_k;
                int bitpos = 1;        /* position of the current bit (1‑based)   */
                int prev   = 0;        /* previous position of a 1 bit             */
                int diff[128];         /* differences between successive 1‑bit positions */
                int diff_len = 0;      /* number of ones encountered              */

                while (tmp) {
                    if (tmp & 1) {
                        int d = bitpos - prev;
                        diff[diff_len++] = d;
                        prev  = bitpos;
                    }
                    tmp >>= 1;
                    bitpos++;
                    /* Avoid overflow of diff array (unbounded k) by limiting */
                    if (diff_len >= 128) break;
                }

                if (diff_len > 1) {
                    /* Reverse diff to obtain the composition parts */
                    int parts[128];
                    for (int i = 0; i < diff_len; ++i) {
                        parts[i] = diff[diff_len - 1 - i];
                    }

                    /* Check duplicated differences between consecutive parts */
                    bool seen[257] = {0};   /* indices 0..256 correspond to differences -128..128 */
                    for (int i = 0; i < diff_len - 1; ++i) {
                        int d = parts[i + 1] - parts[i];
                        int idx = d + 128;     /* shift to non‑negative index */
                        if (seen[idx]) {
                            has_dup = true;
                            break;
                        }
                        seen[idx] = true;
                    }
                }
            }

            if (has_dup) {
                cache[cache_count++] = next_k;
            }
            next_k++;
        }
    }

    return cache[n - 1];
}
