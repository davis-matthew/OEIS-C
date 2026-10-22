/*
 * A304231  Numbers of the form m*k with m <= k < 2m.
 * https://oeis.org/A304231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A304231 – Numbers of the form m*k with m <= k < 2*m.  */
#include <stdint.h>
#include <stdbool.h>

/* Element kept in the heap: current value, its m , current k. */
typedef struct {
    seq_t  val;   /* m*k */
    idx_t  m;     /* the smaller factor       */
    idx_t  k;     /* the larger factor (>=m)  */
} node_t;

/* ---------- Tiny binary min‑heap ---------------------------------------*/
static void heap_push(node_t heap[], idx_t *sz, node_t x)
{
    idx_t i = (*sz)++;
    heap[i] = x;
    /* sift up */
    while (i > 0) {
        idx_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        node_t tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

static node_t heap_pop(node_t heap[], idx_t *sz)
{
    node_t top = heap[0];
    (*sz)--;
    if (*sz > 0) {
        heap[0] = heap[*sz];
        idx_t i = 0;
        while (true) {
            idx_t l = (i << 1) + 1;
            idx_t r = l + 1;
            idx_t small = i;
            if (l < *sz && heap[l].val < heap[small].val) small = l;
            if (r < *sz && heap[r].val < heap[small].val) small = r;
            if (small == i) break;
            node_t tmp = heap[i];
            heap[i] = heap[small];
            heap[small] = tmp;
            i = small;
        }
    }
    return top;
}
/* ---------------------------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n <= 0, return 0 */

    /* ------------------------------------------------------------------
       We generate the sequence for all m up to MAX_M.  
       MAX_M is chosen large enough that the first 10 000 distinct elements
       are produced (sum_{m=1}^{MAX_M} m >= 2×10 000). */
    const idx_t MAX_M = 400;          /* 400*401/2 = 80200 pairs */

    node_t heap[MAX_M];
    idx_t heap_sz = 0;

    /* push the first element of each m (k = m) */
    for (idx_t m = 1; m <= MAX_M; ++m) {
        node_t nd = { (seq_t)m * (seq_t)m, m, m };
        heap_push(heap, &heap_sz, nd);
    }

    seq_t last_val = 0;
    bool first = true;
    idx_t count = 0;

    while (count < n && heap_sz > 0) {
        node_t cur = heap_pop(heap, &heap_sz);

        /* skip duplicates */
        if (first || cur.val != last_val) {
            ++count;
            last_val = cur.val;
            if (count == n) break;
            first = false;
        }

        /* push the next k for this m, if still allowed */
        if (cur.k < 2 * cur.m - 1) {
            node_t nxt = { (seq_t)cur.m * (seq_t)(cur.k + 1), cur.m, cur.k + 1 };
            heap_push(heap, &heap_sz, nxt);
        }
    }

    return last_val;
}
