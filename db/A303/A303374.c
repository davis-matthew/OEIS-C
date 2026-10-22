/*
 * A303374  Numbers of the form a^4 + b^6, with integers a, b > 0.
 * https://oeis.org/A303374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   node_val_lt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth smallest integer that can be written as a^4 + b^6 with a,b > 0. */
/* The algorithm generates the sums in ascending order using a min‑heap over the
 * sequences for each fixed b.  It keeps a static cache of the generated
 * values so that repeated calls with arbitrary indices are answered in O(1). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/node_val_lt.h"

/* Node structure representing one entry in the heap: value of a^4 + b^6,
 * current a, fixed b, and pre‑computed b^6 to speed up generation of the
 * next term for the same b. */
struct Node {
    seq_t val;    /* a^4 + b^6 */
    uint64_t a;   /* current a */
    uint64_t b;   /* fixed b */
    seq_t b6;     /* pre‑computed b^6 */
};

/* ---------- Min‑heap implementation ---------- */
static struct Node *heap_arr = NULL;   /* array of heap nodes */
static size_t heap_size = 0;           /* current number of nodes in heap */
static size_t heap_cap  = 0;           /* allocated capacity of heap_arr */

/* Swaps two nodes in the heap array. */
static void heap_swap(struct Node *x, struct Node *y) {
    struct Node t = *x; *x = *y; *y = t;
}

/* Pushes a new node into the heap and restores heap property. */
static void heap_push(const struct Node *node) {
    if (heap_size == heap_cap) {                /* grow heap if needed */
        size_t new_cap = heap_cap ? heap_cap * 2 : 64;
        struct Node *new_arr = (struct Node*)realloc(heap_arr,
                                                    new_cap * sizeof(struct Node));
        if (!new_arr) exit(1);
        heap_arr = new_arr;
        heap_cap = new_cap;
    }
    heap_arr[heap_size] = *node;
    size_t i = heap_size++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap_arr[p].val <= heap_arr[i].val) break;
        heap_swap(&heap_arr[p], &heap_arr[i]);
        i = p;
    }
}

/* Pops the smallest node from the heap and restores heap property. */
static struct Node heap_pop(void) {
    struct Node root = heap_arr[0];
    heap_arr[0] = heap_arr[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_size && heap_arr[l].val < heap_arr[smallest].val)
            smallest = l;
        if (r < heap_size && heap_arr[r].val < heap_arr[smallest].val)
            smallest = r;
        if (smallest == i) break;
        heap_swap(&heap_arr[i], &heap_arr[smallest]);
        i = smallest;
    }
    return root;
}

/* ---------- Result cache implementation ---------- */
static seq_t *cache = NULL;         /* array of generated sequence values */
static size_t cache_len = 0;        /* number of values stored in cache */
static size_t cache_cap = 0;        /* allocated capacity of cache */

/* Grows the cache array when needed. */
static void cache_grow(void) {
    size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- Generate values until at least n terms are available ---------- */
static void generate_to(idx_t n) {
    /* On first call, initialise the heap with the first term (a=1)
     * for each b up to a fixed maximum.  200 is safely larger than the
     * b needed for the first 10 000 terms. */
    if (heap_arr == NULL) {
        const uint64_t MAX_B = 200;
        heap_cap = MAX_B + 10;
        heap_arr = (struct Node*)malloc(heap_cap * sizeof(struct Node));
        if (!heap_arr) exit(1);
        heap_size = 0;
        for (uint64_t b = 1; b <= MAX_B; ++b) {
            unsigned __int128 tb = (unsigned __int128)b * b * b * b * b * b;
            seq_t b6 = (seq_t)tb;                     /* fits in 64 bits for b<=200 */
            struct Node nd = { .val = b6 + (seq_t)1, .a = 1, .b = b, .b6 = b6 };
            heap_push(&nd);
        }
    }
    /* Grow cache storage if necessary. */
    while (cache_len < n) {
        struct Node cur = heap_pop();

        /* Append to cache unless it is the same as the previous value. */
        if (cache_len == 0 || cur.val != cache[cache_len - 1]) {
            if (cache_len == cache_cap) cache_grow();
            cache[cache_len++] = cur.val;
        }

        /* Generate the next term for the same b. */
        uint64_t a_next = cur.a + 1;
        unsigned __int128 a4 = (unsigned __int128)a_next *
                               a_next *
                               a_next *
                               a_next;
        unsigned __int128 val128 = a4 + (unsigned __int128)cur.b6;

        /* Avoid overflow on 64‑bit builds. */
        if (sizeof(seq_t) <= 8 &&
            val128 > (unsigned __int128)UINT64_MAX)
            continue;          /* skip pushing terms that would overflow */

        struct Node nxt = { .val = (seq_t)val128,
                            .a   = a_next,
                            .b   = cur.b,
                            .b6  = cur.b6 };
        heap_push(&nxt);
    }
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but guard anyway */
    generate_to(n);
    return cache[n - 1];
}
