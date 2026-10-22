/*
 * A355680  Numerator generator for offsets from the quarter points of the
 *          Cantor ternary set to the center points of deleted middle thirds:
 *          1 is in the list and if m is in the list -3m-4 and -3m+4 are in
 *          the list, which is ordered by absolute value.
 * https://oeis.org/A355680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A355680: 1 is in the list, and if m is in the list
 * then -3m-4 and -3m+4 are also in the list.  The sequence is the
 * ascending list of all such numbers sorted by absolute value.
 * The function computes the nth term by generating the numbers
 * via a min‑heap over absolute values. */
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Utility: absolute value for seq_t -------------------------------- */
static inline seq_t abs_seq_t(seq_t x)
{
    return x < 0 ? -x : x;
}

/* ------------------------------------------------------------------ */
/* Min‑heap over absolute values ----------------------------------- */
struct heap {
    seq_t *data;        /* array of elements */
    size_t size;        /* current number of elements */
};

static void heap_push(struct heap *h, seq_t v)
{
    size_t i = h->size++;
    h->data[i] = v;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (abs_seq_t(h->data[p]) <= abs_seq_t(h->data[i]))
            break;
        seq_t tmp = h->data[p];
        h->data[p] = h->data[i];
        h->data[i] = tmp;
        i = p;
    }
}

static seq_t heap_pop(struct heap *h)
{
    seq_t top = h->data[0];
    --h->size;
    if (h->size == 0)
        return top;
    h->data[0] = h->data[h->size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= h->size)
            break;
        size_t r = l + 1;
        size_t best = l;
        if (r < h->size && abs_seq_t(h->data[r]) < abs_seq_t(h->data[l]))
            best = r;
        if (abs_seq_t(h->data[i]) <= abs_seq_t(h->data[best]))
            break;
        seq_t tmp = h->data[i];
        h->data[i] = h->data[best];
        h->data[best] = tmp;
        i = best;
    }
    return top;
}

/* ------------------------------------------------------------------ */
/* Main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n==0, but guard anyway */

    /* capacity needs to hold at most 2*n+10 elements. */
    size_t cap = (size_t)n * 2 + 10;
    seq_t *buf = (seq_t *)malloc(sizeof(seq_t) * cap);
    if (!buf)
        return 0;  /* allocation failure fallback */

    struct heap h = { buf, 0 };
    heap_push(&h, (seq_t)1);          /* start with the root 1 */

    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t m = heap_pop(&h);
        if (i == n)
            result = m;

        /* generate children: -3*m-4 and -3*m+4 */
        seq_t c1 = -3 * m - 4;
        seq_t c2 = -3 * m + 4;

        /* skip any child that equals the parent (avoids duplicate 1) */
        if (c1 != m)
            heap_push(&h, c1);
        if (c2 != m)
            heap_push(&h, c2);
    }

    free(buf);
    return result;
}
