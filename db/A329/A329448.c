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
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------- includes ---------------- */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/mem.h"

/* Increase digit occurrence counts for a term */
static void add_counts(unsigned long long cnt[10], seq_t x)
{
    if (x == 0) {
        cnt[0] += 1;
        return;
    }
    while (x != 0) {
        unsigned int d = (unsigned int)(x % 10);
        cnt[d] += 1;
        x /= 10;
    }
}

/* Test if the sequence starting from 'start' contains 'start' again. */
static bool repeats(seq_t start)
{
    unsigned long long cnt[10] = {0};
    add_counts(cnt, start);

    unsigned int lastd = (unsigned int)(start % 10);
    seq_t leading_part = start / 10;

    while (1) {
        /* Find least frequent non‑zero digit (min count, tie → smaller digit) */
        unsigned int d = 0;
        unsigned long long best = ULLONG_MAX;
        for (int i = 0; i < 10; ++i) {
            if (cnt[i] > 0 && cnt[i] < best) {
                best = cnt[i];
                d = (unsigned int)i;
            }
        }

        seq_t next = (seq_t)10 * (seq_t)best + d;

        add_counts(cnt, next);

        if (next == start) return true;

        if (cnt[lastd] > leading_part) return false;
    }
}

/* ------------- helpers ------------------ */

/* Double array capacity to at least 'need' elements */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 128;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
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
