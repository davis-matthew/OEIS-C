/*
 * A255631  Number of n-length words on {0,1,2,3} avoiding runs of zeros of
 *          length 1 (mod 3).
 * https://oeis.org/A255631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 3*a(n+2) + 4*a(n), with a(0)=1, a(1)=3, a(2)=10
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 930 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;
    static size_t cap = 0;
    static idx_t computed = 0;  /* number of elements stored 0..computed-1 */

    /* Initialise the base values once */
    if (computed == 0) {
        /* Ensure capacity for the first three terms */
        grow_seq(&arr, &cap, 3);
        arr[0] = (seq_t)1;
        arr[1] = (seq_t)3;
        arr[2] = (seq_t)10;
        computed = 3;
    }

    /* Extend the table if necessary */
    if (computed <= n) {
        grow_seq(&arr, &cap, (size_t)(n + 1));
        for (idx_t j = computed; j <= n; ++j) {
            /* Recurrence holds for j ≥ 3 */
            arr[j] = 3 * arr[j - 1] + 4 * arr[j - 3];
        }
        computed = n + 1;
    }

    return arr[n];
}
