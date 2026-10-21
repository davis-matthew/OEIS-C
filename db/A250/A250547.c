/*
 * A250547  Numbers of the form 6^x + y^6 with x, y >= 0.
 * https://oeis.org/A250547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of the form 6^x + y^6 with x, y >= 0 (sorted
 *            ascending)
 * Range:     u8 n <= 11, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_bigint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_bigint.h"

/* node used in the min‑heap */
typedef struct {
    seq_t sum;
    idx_t x;
    idx_t y;
} node_t;

/* ---------- static global data ---------- */
static node_t *heap      = NULL;
static size_t  heap_cap  = 0;
static size_t  heap_len  = 0;

static seq_t *seq_arr = NULL;
static size_t seq_cap  = 0;
static size_t seq_len  = 0;

static seq_t *pow6_arr = NULL;
static size_t pow6_cap = 0;
static size_t pow6_len = 0;

static seq_t *powy_arr = NULL;
static size_t powy_cap = 0;
static size_t powy_len = 0;

/* Visited table for (x, y) pairs – enough for n <= 10000 */
#define VISX 256
#define VISY 2048
static bool visited[VISX][VISY] = {0};

static const seq_t MAX_VAL = (seq_t)-1;   /* maximum unsigned value of seq_t */

/* ---------- utility functions ---------- */

/* heap push (maintains min‑heap on sum) */
static void heap_push(node_t n) {
    if (heap_len == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 4;
        node_t *tmp = (node_t *)realloc(heap, new_cap * sizeof(node_t));
        heap = tmp;
        heap_cap = new_cap;
    }
    heap[heap_len] = n;
    size_t i = heap_len++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].sum <= heap[i].sum) break;
        node_t t = heap[p]; heap[p] = heap[i]; heap[i] = t;
        i = p;
    }
}

/* heap pop (returns minimum) */
static node_t heap_pop(void) {
    node_t r = heap[0];
    heap[0] = heap[heap_len - 1];
    --heap_len;
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t rght = l + 1;
        size_t smallest = i;
        if (l < heap_len && heap[l].sum < heap[smallest].sum) smallest = l;
        if (rght < heap_len && heap[rght].sum < heap[smallest].sum) smallest = rght;
        if (smallest == i) break;
        node_t t = heap[i]; heap[i] = heap[smallest]; heap[smallest] = t;
        i = smallest;
    }
    return r;
}

/* ensure pow6_arr contains value for index x */
static void ensure_pow6(idx_t x) {
    while ((size_t)x >= pow6_len) {
        if (pow6_len == 0) break;   /* no previous value */
        if (!is_bigint() && pow6_arr[pow6_len - 1] > MAX_VAL / (seq_t)6) return;
        seq_t nv = pow6_arr[pow6_len - 1] * (seq_t)6;
        if (pow6_cap <= pow6_len) {
            size_t nc = pow6_cap ? pow6_cap * 2 : 4;
            while (nc <= pow6_len) nc <<= 1;
            seq_t *tmp = (seq_t *)realloc(pow6_arr, nc * sizeof(seq_t));
            pow6_arr = tmp; pow6_cap = nc;
        }
        pow6_arr[pow6_len] = nv;
        ++pow6_len;
    }
}

/* ensure powy_arr contains value for index y (y^6) */
static void ensure_powy(idx_t y) {
    while ((size_t)y >= powy_len) {
        if (y == 0) {
            seq_t nv = 0;
            if (powy_cap <= powy_len) {
                size_t nc = powy_cap ? powy_cap * 2 : 4;
                while (nc <= powy_len) nc <<= 1;
                seq_t *tmp = (seq_t *)realloc(powy_arr, nc * sizeof(seq_t));
                powy_arr = tmp; powy_cap = nc;
            }
            powy_arr[powy_len] = nv;
            ++powy_len;
            continue;
        }
        seq_t r = 1;
        bool over = false;
        for (int i = 0; i < 6; ++i) {
            if (!is_bigint() && r > MAX_VAL / (seq_t)y) { over = true; break; }
            r *= (seq_t)y;
        }
        if (over) return;    /* cannot compute further values without overflow */
        if (powy_cap <= powy_len) {
            size_t nc = powy_cap ? powy_cap * 2 : 4;
            while (nc <= powy_len) nc <<= 1;
            seq_t *tmp = (seq_t *)realloc(powy_arr, nc * sizeof(seq_t));
            powy_arr = tmp; powy_cap = nc;
        }
        powy_arr[powy_len] = r;
        ++powy_len;
    }
}

/* push a candidate (x, y) into the heap if it has not been seen before */
static void push_candidate(idx_t nx, idx_t ny) {
    if (nx >= VISX || ny >= VISY) return;
    ensure_pow6(nx);
    ensure_powy(ny);
    if ((size_t)nx >= pow6_len || (size_t)ny >= powy_len) return;

    seq_t s = pow6_arr[nx] + powy_arr[ny];
    if (!is_bigint() && (s < pow6_arr[nx] || s < powy_arr[ny])) return; /* overflow */

    if (visited[nx][ny]) return;
    visited[nx][ny] = true;
    heap_push((node_t){s, nx, ny});
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    /* initialize globals once */
    static bool initialized = false;
    if (!initialized) {
        initialized = true;

        /* pow6[0] = 1, powy[0] = 0 */
        pow6_cap = 4; pow6_len = 1;
        pow6_arr = (seq_t *)malloc(pow6_cap * sizeof(seq_t));
        pow6_arr[0] = (seq_t)1;

        powy_cap = 4; powy_len = 1;
        powy_arr = (seq_t *)malloc(powy_cap * sizeof(seq_t));
        powy_arr[0] = (seq_t)0;

        /* heap initial node (0,0) */
        heap_cap = 4; heap_len = 1;
        heap = (node_t *)malloc(heap_cap * sizeof(node_t));
        heap[0].sum = (seq_t)1;
        heap[0].x   = 0;
        heap[0].y   = 0;
        visited[0][0] = true;
    }

    /* generate terms up to the requested index */
    while (seq_len < n) {
        node_t cur = heap_pop();
        seq_t val = cur.sum;

        /* store if new */
        if (seq_len == 0 || val != seq_arr[seq_len - 1]) {
            if (seq_len == seq_cap) {
                size_t nc = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq_arr, nc * sizeof(seq_t));
                seq_arr = tmp; seq_cap = nc;
            }
            seq_arr[seq_len++] = val;
        }

        /* push neighbours (x+1, y) and (x, y+1) */
        push_candidate(cur.x + 1, cur.y);
        push_candidate(cur.x, cur.y + 1);
    }

    return seq_arr[n - 1];
}
