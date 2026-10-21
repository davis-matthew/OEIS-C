/*
 * A247336  Numbers of the form m^k - k - 1 with k > 0 and m > 1.
 * https://oeis.org/A247336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247336: Numbers of the form m^k - k - 1,
   with m > 1 and k > 1, together with the special term 0.
   The sequence is generated in increasing order, removing duplicates.
   The function supports arbitrary-precision seq_t (or any unsigned integer type).
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp) */

typedef struct {
    seq_t val;   /* current value m^k - k - 1 */
    idx_t  k;    /* exponent (>=2) */
    idx_t  m;    /* current base (>=2) */
} node_t;

/* binary min‑heap for node_t, heap pointer is modified by push/pop */
static void heap_push(node_t **heap, size_t *size, size_t *cap,
                      const node_t *x)
{
    if (*size >= *cap) {
        *cap = (*cap == 0) ? 16 : (*cap << 1);
        *heap = (node_t *)realloc(*heap, *cap * sizeof(node_t));
    }
    size_t i = (*size)++;
    node_t tmp = *x;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if ((*heap)[p].val <= tmp.val) break;
        (*heap)[i] = (*heap)[p];
        i = p;
    }
    (*heap)[i] = tmp;
}

static void heap_pop(node_t **heap, size_t *size, node_t *x)
{
    size_t i = 0;
    size_t last = --(*size);
    *x = (*heap)[0];
    node_t tmp = (*heap)[last];
    while (1) {
        size_t l = (i << 1) + 1;
        if (l > last) break;
        size_t r = l + 1;
        size_t c = (r <= last && (*heap)[r].val < (*heap)[l].val) ? r : l;
        if (tmp.val <= (*heap)[c].val) break;
        (*heap)[i] = (*heap)[c];
        i = c;
    }
    (*heap)[i] = tmp;
}

/* Cached sequence values (a(1) corresponds to cache[0]) */
static seq_t  *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Min‑heap of unfinished nodes */
static node_t *heap     = NULL;
static size_t heap_size = 0;
static size_t heap_cap  = 0;
static bool   heap_init = false;

/* Fill cache with at least n terms (a(1)…a(n)) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    /* Initialise cache with the first term 0 (a(1) = 0) */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Initialise the heap once (k >= 2) */
    if (!heap_init) {
        const idx_t K_INIT = 30;          /* should cover n up to ≈ 10000 */
        heap_cap   = 64;
        heap       = (node_t *)malloc(heap_cap * sizeof(node_t));
        heap_size  = 0;
        for (idx_t k = 2; k <= K_INIT; ++k) {
            seq_t val = big_pow((seq_t)2, (seq_t)k);
            val -= (seq_t)k;
            val -= (seq_t)1;
            node_t nxx = { val, k, 2 };
            heap_push(&heap, &heap_size, &heap_cap, &nxx);
        }
        heap_init = true;
    }

    seq_t last_val;
    bool   last_set = false;

    while (cache_len < (size_t)n && heap_size > 0) {
        node_t cur;
        heap_pop(&heap, &heap_size, &cur);

        /* Store unique value */
        if (!last_set || cur.val != last_val) {
            if (cache_len >= cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cur.val;
            last_val = cur.val;
            last_set = true;
        }

        /* Push next base for the same exponent k */
        node_t nxt = { 0, cur.k, cur.m + 1 };
        nxt.val = big_pow((seq_t)nxt.m, (seq_t)nxt.k);
        nxt.val -= (seq_t)nxt.k;
        nxt.val -= (seq_t)1;
        heap_push(&heap, &heap_size, &heap_cap, &nxt);
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* index < offset is undefined */
    ensure_cache(n);
    return cache[n - 1];
}
