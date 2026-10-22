/*
 * A340047  Numbers that are the sum of a fourth power s and a square t such
 *          that 0 < s < t.
 * https://oeis.org/A340047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by generating all a^4 + b^2 with a ≥ 1, b > a^2 and
   returning the n‑th smallest distinct value. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* safety – indices are 1‑based */

    /* A min‑heap of candidate sums.  Using n entries is sufficient
       for n up to at least 10 000. */
    size_t heap_cap = (size_t)n;
    struct node { seq_t sum; idx_t a; idx_t b; seq_t a4; } *heap =
        (struct node *)malloc(heap_cap * sizeof(struct node));
    if (!heap) return 0;  /* allocation failure – should not occur */

    /* Build the initial heap: one entry per a = 1 .. n. */
    size_t hsize = 0;
    for (idx_t ai = 1; ai <= n; ++ai) {
        seq_t a2 = (seq_t)ai * (seq_t)ai;
        seq_t a4 = a2 * a2;
        /* Minimal integer b with a^4 < b^2 is b = a^2 + 1. */
        idx_t b = (idx_t)a2 + 1;
        seq_t sum = a4 + (seq_t)b * (seq_t)b;

        heap[hsize].sum = sum;
        heap[hsize].a   = ai;
        heap[hsize].b   = b;
        heap[hsize].a4  = a4;
        ++hsize;

        /* sift‑up to maintain min‑heap property */
        size_t i = hsize - 1;
        while (i > 0) {
            size_t p = (i - 1) >> 1;
            if (heap[p].sum <= heap[i].sum) break;
            struct node tmp = heap[p];
            heap[p] = heap[i];
            heap[i] = tmp;
            i = p;
        }
    }

    seq_t last = 0;
    bool have_last = false;
    seq_t result = 0;
    size_t count = 0;

    while (count < n) {
        /* Extract the smallest sum from the heap. */
        struct node top = heap[0];
        heap[0] = heap[--hsize];

        /* sift‑down to restore heap property */
        size_t i = 0;
        while (1) {
            size_t l = (i << 1) + 1;
            size_t r = l + 1;
            size_t smallest = i;
            if (l < hsize && heap[l].sum < heap[smallest].sum) smallest = l;
            if (r < hsize && heap[r].sum < heap[smallest].sum) smallest = r;
            if (smallest == i) break;
            struct node tmp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = tmp;
            i = smallest;
        }

        /* Emit distinct sums only. */
        if (!have_last || top.sum != last) {
            last = top.sum;
            have_last = true;
            ++count;
            result = last;
        }

        /* Insert the next candidate for the same a. */
        idx_t newb = top.b + 1;
        seq_t newsum = top.a4 + (seq_t)newb * (seq_t)newb;

        heap[hsize].sum = newsum;
        heap[hsize].a   = top.a;
        heap[hsize].b   = newb;
        heap[hsize].a4  = top.a4;
        ++hsize;

        /* sift‑up for the new element */
        size_t j = hsize - 1;
        while (j > 0) {
            size_t p = (j - 1) >> 1;
            if (heap[p].sum <= heap[j].sum) break;
            struct node tmp = heap[p];
            heap[p] = heap[j];
            heap[j] = tmp;
            j = p;
        }
    }

    free(heap);
    return result;
}
