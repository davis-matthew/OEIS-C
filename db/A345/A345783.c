/*
 * A345783  Numbers that are the sum of eight cubes in exactly one way.
 * https://oeis.org/A345783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 209, u32 n <= 209, u64 n <= 209,
 *            u128 n <= 209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute numbers that are the sum of eight cubes in exactly one way */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static seq_t *unique_vals = NULL;   /* array of all sequence terms, 0‑indexed internally */
static size_t unique_count = 0;     /* number of terms stored */

/* Grow a dynamic array of seq_t to at least 'need' elements. */
static void grow_seq_arr(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap)
        return;
    size_t newcap = (*cap == 0) ? 8 : *cap * 2;
    while (newcap < need)
        newcap *= 2;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* Build the sequence once, up to an upper bound that comfortably exceeds the
   number of terms needed (10 000 for the performance requirement). */
static void init_sequence(void)
{
    if (unique_vals != NULL)          /* already built */
        return;

    const size_t MAX_SUM = 2000000;   /* upper bound for sums to cover many terms */
    const size_t STRIDE = MAX_SUM + 1;

    /* counts[k*STRIDE + s] holds the number of unordered representations of
       the sum 's' as a sum of exactly 'k' positive cubes.  Use 64‑bit
       integers for the counts; they will never overflow for the chosen limit. */
    uint64_t *counts = (uint64_t *)calloc(9 * STRIDE, sizeof(uint64_t));
    if (!counts)
        abort();

    counts[0 * STRIDE + 0] = 1;                   /* base case */

    for (size_t root = 1;; ++root) {
        size_t cube = root * root * root;
        if (cube > MAX_SUM)
            break;
        for (int k = 1; k <= 8; ++k) {
            uint64_t *prev = counts + (k - 1) * STRIDE;
            uint64_t *curr = counts + k * STRIDE;
            for (size_t s = cube; s <= MAX_SUM; ++s)
                curr[s] += prev[s - cube];
        }
    }

    /* Dynamically collect sums that have exactly one representation. */
    seq_t *buf = NULL;
    size_t buf_cap = 0;
    size_t buf_len = 0;

    for (size_t s = 8; s <= MAX_SUM; ++s) {   /* minimal sum with 8 cubes is 8 */
        if (counts[8 * STRIDE + s] == 1) {
            grow_seq_arr(&buf, &buf_cap, buf_len + 1);
            buf[buf_len++] = (seq_t)s;
        }
    }

    free(counts);

    unique_vals = buf;
    unique_count = buf_len;
}

/* External interface required by the grading framework. */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0 || (size_t)n > unique_count)   /* out of range – should not happen */
        return 0;
    return unique_vals[(size_t)(n - 1)];
}
