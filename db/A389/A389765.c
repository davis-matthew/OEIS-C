/*
 * A389765  Odd sub-perfect powers: even perfect powers minus 1.
 * https://oeis.org/A389765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A075090(n) – 1 (even perfect powers minus 1)
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 619, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the nth even perfect power subtracting 1. */
/* Works for any seq_t/idx_t type. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    seq_t val;   /* current value a^k                     */
    seq_t base;  /* base a (even)                         */
    idx_t exp;   /* exponent k (≥ 2)                      */
} Node;

static void sift_up(Node *h, size_t i)
{
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h[i].val >= h[p].val) break;
        Node t = h[i]; h[i] = h[p]; h[p] = t;
        i = p;
    }
}

static void sift_down(Node *h, size_t i, size_t sz)
{
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= sz) break;
        size_t sm = l;
        size_t r = l + 1;
        if (r < sz && h[r].val < h[l].val) sm = r;
        if (h[i].val <= h[sm].val) break;
        Node t = h[i]; h[i] = h[sm]; h[sm] = t;
        i = sm;
    }
}

static void heap_push(Node *h, size_t *sz, const Node *node)
{
    h[*sz] = *node;
    sift_up(h, *sz);
    (*sz)++;
}

static void heap_pop(Node *h, size_t *sz, Node *out)
{
    *out = h[0];
    (*sz)--;
    if (*sz > 0) {
        h[0] = h[*sz];
        sift_down(h, 0, *sz);
    }
}

/* large enough for n up to 10000 (more than needed) */
static const size_t HEAP_CAP = 200000;

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n<1; safe guard */

    /* allocate heap array */
    Node *heap;
#ifdef __cplusplus
    heap = new Node[HEAP_CAP];
#else
    heap = (Node*)malloc(HEAP_CAP * sizeof(Node));
#endif
    size_t heap_sz = 0;
    size_t heap_cap = HEAP_CAP;

    /* start with even base 2 (2^2 = 4) */
    Node start;
    start.base = (seq_t)2;
    start.exp  = (idx_t)2;
    start.val  = start.base * start.base;
    heap_push(heap, &heap_sz, &start);

    seq_t next_base = (seq_t)4;          /* next even base to consider   */
    seq_t last_val  = 0;
    bool   first    = true;
    idx_t  count    = 0;

    while (1) {
        /* insert new bases whose square <= current smallest */
        while (next_base > 0) {
            seq_t cand = next_base * next_base;
            if (cand > heap[0].val) break;
            Node nb;
            nb.base = next_base;
            nb.exp  = (idx_t)2;
            nb.val  = cand;
            heap_push(heap, &heap_sz, &nb);
            next_base += (seq_t)2;      /* advance to next even number */
        }

        /* extract smallest power */
        Node cur;
        heap_pop(heap, &heap_sz, &cur);

        /* count distinct values only */
        if (first || cur.val != last_val) {
            count++;
            if (count == n) {
                seq_t res = cur.val - (seq_t)1;
#ifdef __cplusplus
                delete [] heap;
#else
                free(heap);
#endif
                return res;
            }
            last_val = cur.val;
            first = false;
        }

        /* push next exponent for this base */
        Node nxt;
        nxt.base = cur.base;
        nxt.exp  = cur.exp + (idx_t)1;
        nxt.val  = cur.val * cur.base;
        heap_push(heap, &heap_sz, &nxt);
    }
    /* unreachable but keep compiler happy */
#ifdef __cplusplus
    delete [] heap;
#else
    free(heap);
#endif
    return 0;
}
