/*
 * A347254  Positive integers k such that 10*k+6 is equal to the product of
 *          two integers ending with 4 (A347253).
 * https://oeis.org/A347254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (integer k for which 10*k+6 = (10*a+4)*(10*b+4), k =
 *            10*a*b+4(a+b)+1)
 * Range:     u8 n <= 21, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* --- data structures for the priority queue -------------------------------- */

typedef struct {
    seq_t k;       /* current value  */
    idx_t a;       /* fixed 'a' in the pair  */
    idx_t b;       /* counter for 'b'  */
} Node;

static Node *h = NULL;        /* heap array           */
static size_t h_size = 0;     /* number of nodes in heap */
static size_t h_cap   = 0;    /* allocated capacity     */

static void heap_grow(void) {
    size_t new_cap = (h_cap == 0) ? 8 : h_cap * 2;
    Node *tmp = (Node *)realloc(h, new_cap * sizeof(Node));
    if (!tmp) abort();          /* out of memory is fatal */
    h = tmp;
    h_cap = new_cap;
}

static void heap_push(Node v) {
    if (h_size == h_cap) heap_grow();
    size_t i = h_size++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h[p].k <= v.k) break;
        h[i] = h[p];
        i = p;
    }
    h[i] = v;
}

static Node heap_pop(void) {
    Node res = h[0];
    h[0] = h[--h_size];
    /* sift down */
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= h_size) break;
        size_t r = l + 1;
        size_t s = (r < h_size && h[r].k < h[l].k) ? r : l;
        if (h[s].k >= h[i].k) break;
        Node tmp = h[i];
        h[i] = h[s];
        h[s] = tmp;
        i = s;
    }
    return res;
}

/* --- cache for the sequence ----------------------------------------------- */

static seq_t *cache = NULL;  /* sorted distinct terms */
static size_t cache_size = 0;
static size_t cache_cap   = 0;

static void cache_grow(void) {
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* --- incremental generator ----------------------------------------------- */

static idx_t next_a_to_seed = 0;   /* next unseeded 'a' */
static seq_t last_k = 0;          /* last value inserted into cache (for duplicate skip) */

/* Extend the cache until it contains at least 'need' terms. */
static void extend_cache_to(idx_t need) {
    while (cache_size < (size_t)need) {
        /* Seed new a's whose smallest k is <= current heap minimum */
        while (1) {
            seq_t min_k = (seq_t)4 * next_a_to_seed + 1;
            if (h_size == 0 || min_k <= h[0].k) {
                /* push the first term of this a-sequence */
                Node v;
                v.a = next_a_to_seed;
                v.b = 0;
                v.k = min_k;
                heap_push(v);
                next_a_to_seed++;
            } else {
                break;
            }
        }

        /* Pop the smallest term from the heap */
        Node cur = heap_pop();

        /* Store in cache if not a duplicate of previous */
        if (cache_size == 0 || cur.k != last_k) {
            if (cache_size == cache_cap) cache_grow();
            cache[cache_size++] = cur.k;
            last_k = cur.k;
        }

        /* Push the next term of the same a-sequence */
        cur.b += 1;
        cur.k += (seq_t)10 * cur.a + 4;
        heap_push(cur);
    }
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* sequence is 1-indexed; 0 is undefined */
    if (cache_size < (size_t)n) extend_cache_to(n);
    return cache[n - 1];
}
