/*
 * A397957  Polydivisible numbers in base 7.
 * https://oeis.org/A397957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 293, u32 n <= 962, u64 n <= 1068,
 *            u128 n <= 1068, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Algorithm: generate the n‑th polydivisible number in base 7.
   a(n) is the n‑th smallest integer whose base‑7 representation
   satisfies that the first k digits form a number divisible by k
   for every position k.  The sequence is indexed from 1.  */

/* Node of the min‑heap: the value and the current length (number of 7‑digits). */
typedef struct {
    seq_t val;
    idx_t len;
} Node;

/* ---------------  dynamic cache of generated values --------------- */
static seq_t *cache      = NULL;
static size_t   cache_size = 0;
static size_t   cache_cap  = 0;

static void cache_append(seq_t v)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = v;
}

/* ---------------  min‑heap of candidate numbers --------------- */
static Node *heap      = NULL;
static size_t   heap_size = 0;
static size_t   heap_cap  = 0;

static void heap_push(Node n)
{
    if (heap_size == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 16;
        Node *tmp = (Node *)realloc(heap, new_cap * sizeof(Node));
        heap = tmp;
        heap_cap = new_cap;
    }
    size_t i = heap_size++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val < n.val) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i] = n;
}

static Node heap_pop(void)
{
    Node root = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= heap_size) break;
        size_t r = l + 1;
        size_t s = (r < heap_size && heap[r].val < heap[l].val) ? r : l;
        if (heap[s].val >= heap[i].val) break;
        Node tmp = heap[i];
        heap[i] = heap[s];
        heap[s] = tmp;
        i = s;
    }
    return root;
}

/* ---------------  generate values until we have at least n of them --------------- */
static void generate_until(size_t n)
{
    if (cache_size >= n) return;

    /* initialise heap with single‑digit numbers (0 … 6) only once */
    if (heap_size == 0) {
        for (int d = 0; d <= 6; ++d) {
            heap_push((Node){ (seq_t)d, 1 });
        }
    }

    while (cache_size < n) {
        Node cur = heap_pop();
        cache_append(cur.val);

        /* 0 is the only single digit that must not be expanded further
           otherwise we would generate duplicate zeros. */
        if (cur.val == (seq_t)0) continue;

        idx_t new_len = cur.len + 1;
        seq_t base = cur.val * (seq_t)7;
        for (int d = 0; d <= 6; ++d) {
            seq_t new_val = base + (seq_t)d;
            if (new_val % (seq_t)new_len == 0) {
                heap_push((Node){ new_val, new_len });
            }
        }
    }
}

/* --------------------  public function --------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset is 1 – behaviour below is ignored */
    generate_until((size_t)n);
    return cache[(size_t)n - 1];
}
