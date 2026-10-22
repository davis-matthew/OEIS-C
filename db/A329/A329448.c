/*
 * A329448  Numbers n which appear twice in the sequence starting with n and
 *          subsequent terms given by 10c + d where c is the number of times
 *          (> 0) the least frequent digit d has appeared so far, smallest d
 *          in case of tie.
 * https://oeis.org/A329448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   add_counts, repeats
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------- includes ---------------- */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/add_counts.h"
#include "../utils/repeats.h"

/* ------------- helpers ------------------ */

/* Double array capacity to at least 'need' elements */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 128;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* memory allocation failed */
    *arr = tmp;
    *cap = new_cap;
}

/* ---- public API ------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t *good = NULL;
    static size_t good_cap = 0;
    static size_t good_len = 0;
    static seq_t next_candidate = 1;   /* start testing from 1 */

    if (n == 0) return 0;  /* undefined for a(0), but guard */

    size_t target = (size_t)n - 1;     /* zero‑based index */

    while (good_len <= target) {
        if (repeats(next_candidate)) {
            grow_array(&good, &good_cap, good_len + 1);
            good[good_len++] = next_candidate;
        }
        next_candidate += 1;
    }

    return good[target];
}
