/*
 * A243165  Nonnegative integers of the form 2x^2 + 4xy - 3y^2.
 * https://oeis.org/A243165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   value128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243165: nonnegative integers of the form 2x^2+4xy-3y^2
 * (equivalently 5x^2-2y^2 or 2x^2-5y^2).  The algorithm generates the
 * values for each fixed y as a monotone sequence 2u^2-5y^2 (u = x+y),
 * then merges these sequences with a binary heap, discarding duplicates.
 * It works for any build‑defined seq_t / idx_t pair and stays well under
 * 2 s for n up to about 10000.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/value128.h"

/* ---------- integer square‑root for uint64_t -------------------------------- */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 60;          /* highest power of four <= v */

    while (bit > v) bit >>= 2;
    while (bit != 0) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ---------- minimal u for a given y ------------------------------------- */
static inline idx_t u_start(idx_t y)
{
    /* s = 5*y*y  may need 128‑bit to avoid overflow */
    __uint128_t s = ( __uint128_t )5 * y * y;

    /* Need minimal u with 2*u*u >= s  →  u^2 >= ceil(s/2) */
    idx_t t = (idx_t)((s + 1) >> 1);          /* ceil(s/2) */
    idx_t u = isqrt_u64(t);
    if (( __uint128_t )u * u < t) ++u;
    return u;
}

/* ---------- heap node ----------------------------------------------------- */
typedef struct {
    idx_t          y;   /* fixed y of this sequence                  */
    idx_t          u;   /* current u (x+y) for this y                */
    __uint128_t    val; /* 2u^2 - 5y^2 (current value)                */
} Node;

/* ---------- heap helpers ------------------------------------------------- */
static void sift_down(Node *h, size_t sz, size_t i)
{
    while (1) {
        size_t l = (i << 1) + 1, r = l + 1, s = i;
        if (l < sz && h[l].val < h[s].val) s = l;
        if (r < sz && h[r].val < h[s].val) s = r;
        if (s == i) break;
        Node tmp = h[i]; h[i] = h[s]; h[s] = tmp;
        i = s;
    }
}

static void sift_up(Node *h, size_t i)
{
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h[p].val <= h[i].val) break;
        Node tmp = h[p]; h[p] = h[i]; h[i] = tmp;
        i = p;
    }
}

/* ---------- public routine ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* safety for offset 1 */

    /* Size of the heap: a small constant factor of n is enough for n ≤ 10000 */
    idx_t YMAX = n * 5 + 10;
    if (YMAX < 1000) YMAX = 1000;              /* minimum guard */

    Node *heap = (Node *)malloc((size_t)YMAX * sizeof(Node));
    if (!heap) return 0;                       /* out of memory */

    /* initialise heap with the first element of each y‑sequence */
    for (idx_t y = 0; y < YMAX; ++y) {
        idx_t u0 = u_start(y);
        heap[y].y  = y;
        heap[y].u  = u0;
        heap[y].val = value128(y, u0);
    }

    size_t heap_sz = YMAX;
    for (size_t i = heap_sz / 2; i > 0; --i) sift_down(heap, heap_sz, i - 1);

    /* extraction loop – keep distinct values only */
    __uint128_t last_val = 0;
    int first = 1;
    idx_t  count = 0;
    __uint128_t result = 0;

    while (count < n) {
        Node top = heap[0];
        heap[0] = heap[--heap_sz];
        sift_down(heap, heap_sz, 0);

        if (first || top.val != last_val) {
            last_val = top.val;
            ++count;
            if (count == n) { result = top.val; break; }
            first = 0;
        }

        /* push next u for the same y */
        idx_t next_u = top.u + 1;
        heap[heap_sz].y  = top.y;
        heap[heap_sz].u  = next_u;
        heap[heap_sz].val = value128(top.y, next_u);
        ++heap_sz;
        sift_up(heap, heap_sz - 1);
    }

    free(heap);
    return (seq_t)result;
}
