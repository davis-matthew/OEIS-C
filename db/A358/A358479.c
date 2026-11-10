/*
 * A358479  Number of 6's that appeared in the n-th step when constructing
 *          A030717.
 * https://oeis.org/A358479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   simulation of A030717. Step 1 is the initial list [1]. Each
 *            later step appends, in ascending order of value, the current
 *            frequency of every distinct value in the list. a(n) is the
 *            number of 6's appended in step n, so a(1) = 0
 * Range:     u8 n <= 56, u16 n <= 122, u32 n <= 122, u64 n <= 122,
 *            u128 n <= 122, big any n
 * Validated: partial, pass, 122 terms through n = 122 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A358479_keys = NULL;   /* distinct values in L1, ascending */
static seq_t *A358479_cnts = NULL;   /* their frequencies */
static size_t A358479_size = 0, A358479_cap = 0;
static seq_t *A358479_res = NULL;    /* results per step (index 0 = step 1) */
static size_t A358479_steps = 0, A358479_rcap = 0;

static void A358479_incr(seq_t key)
{
    size_t i = 0;
    while (i < A358479_size && A358479_keys[i] < key) i++;
    if (i < A358479_size && A358479_keys[i] == key) {
        A358479_cnts[i] += 1;
        return;
    }
    if (A358479_size + 1 > A358479_cap) {
        size_t nc = A358479_cap * 2;
        A358479_keys = mem_resize(seq_t, A358479_keys, nc);
        A358479_cnts = mem_resize(seq_t, A358479_cnts, nc);
        A358479_cap = nc;
    }
    for (size_t j = A358479_size; j > i; --j) {
        A358479_keys[j] = A358479_keys[j - 1];
        A358479_cnts[j] = A358479_cnts[j - 1];
    }
    A358479_keys[i] = key;
    A358479_cnts[i] = 1;
    A358479_size++;
}

seq_t a(idx_t n)
{
    if (A358479_cap == 0) {
        A358479_cap = 64;
        A358479_keys = mem_new(seq_t, A358479_cap);
        A358479_cnts = mem_new(seq_t, A358479_cap);
        A358479_rcap = 64;
        A358479_res = mem_new(seq_t, A358479_rcap);
        A358479_size = 0;
        A358479_incr((seq_t)1);
        /* step 1: the initial list [1], which contains no 6 */
        A358479_res[0] = 0;
        A358479_steps = 1;
    }
    while (A358479_steps < (size_t)n) {
        size_t k = A358479_size;
        seq_t *snap = mem_new(seq_t, k);
        for (size_t i = 0; i < k; ++i) snap[i] = A358479_cnts[i];
        seq_t sixes = 0;
        for (size_t i = 0; i < k; ++i) {
            if (snap[i] == 6) sixes++;
            A358479_incr(snap[i]);
        }
        mem_free(snap);
        if (A358479_steps + 1 > A358479_rcap) {
            A358479_rcap *= 2;
            A358479_res = mem_resize(seq_t, A358479_res, A358479_rcap);
        }
        A358479_res[A358479_steps++] = sixes;
    }
    return A358479_res[n - 1];
}
