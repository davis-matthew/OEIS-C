/*
 * A333338  Numbers k such that sigma_2(k) = sigma_2(phi(k)).
 * https://oeis.org/A333338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Odor OEIS A333338 – numbers k such that σ₂(k) = σ₂(φ(k))
    The sequence consists of 1, 7 and the infinite family
    47·11^i·23^j with i,j ≥ 1.  The nth term is produced on‑demand
    by generating the family in ascending order with a binary heap
    and a hash‑based visited table.  Previous terms are cached
    so subsequent calls are O(1).  The implementation works for
    unsigned 8, 16, 32, 64 and arbitrary‑precision seq_t. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* external types supplied by the build */
seq_t phi(idx_t m);
seq_t sigma_k(idx_t m, uint8_t k);

/* --------------------------------------------------------------------- */
/*  Helper structures and static storage                                    */
/* --------------------------------------------------------------------- */

typedef struct {
    seq_t val;
    uint32_t i, j;
} Node;

/* heap for generating the family */
static Node *heap      = NULL;
static size_t heap_size = 0;
static size_t heap_cap  = 0;

/* cache of generated family members (without 1 and 7) */
static seq_t *family    = NULL;
static size_t family_len  = 0;
static size_t family_cap  = 0;

/* hash table for visited (i,j) pairs – simple open addressing */
#define VISITED_CAP 65536U          /* must be power of two */
static uint64_t visited[VISITED_CAP] = {0};

static inline bool visited_insert(uint32_t i, uint32_t j)
{
    /* Returns true if the pair was not seen before, inserts it. */
    uint64_t key = (((uint64_t)i) << 32) | j;
    size_t idx = key & (VISITED_CAP - 1);
    while (visited[idx]) {
        if (visited[idx] == key) return false;   /* already seen */
        idx = (idx + 1) & (VISITED_CAP - 1);
    }
    visited[idx] = key;
    return true;
}

/* --------------------------------------------------------------------- */
/*  Heap operations                                                      */
/* --------------------------------------------------------------------- */

static void heap_init(void)
{
    if (!heap_cap) {
        heap_cap = 64;
        heap = (Node *)malloc(heap_cap * sizeof(Node));
    }
}

static inline void heap_swap(Node *a, Node *b)
{
    Node t = *a;
    *a = *b;
    *b = t;
}

static void heap_push(Node n)
{
    heap_init();
    if (heap_size == heap_cap) {
        heap_cap <<= 1;
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
    }
    size_t k = heap_size++;
    heap[k] = n;
    /* sift up */
    while (k > 0) {
        size_t parent = (k - 1) >> 1;
        if (heap[k].val < heap[parent].val) {
            heap_swap(&heap[k], &heap[parent]);
            k = parent;
        } else break;
    }
}

static Node heap_pop(void)
{
    Node top = heap[0];
    heap[0] = heap[--heap_size];
    /* sift down */
    size_t k = 0;
    while (1) {
        size_t left = (k << 1) + 1;
        size_t right = left + 1;
        size_t smallest = k;
        if (left < heap_size && heap[left].val < heap[smallest].val)
            smallest = left;
        if (right < heap_size && heap[right].val < heap[smallest].val)
            smallest = right;
        if (smallest == k) break;
        heap_swap(&heap[k], &heap[smallest]);
        k = smallest;
    }
    return top;
}

/* --------------------------------------------------------------------- */
/*  Dynamic array for family                                          */
/* --------------------------------------------------------------------- */

static void family_append(seq_t v)
{
    if (family_len == family_cap) {
        family_cap = family_cap ? family_cap << 1 : 16;
        family = (seq_t *)realloc(family, family_cap * sizeof(seq_t));
    }
    family[family_len++] = v;
}

/* --------------------------------------------------------------------- */
/*  Generation routine – builds family up to required target          */
/* --------------------------------------------------------------------- */

static void ensure_family(size_t target)   /* target is 0‑based index in family */
{
    if (family_len > target) return;          /* already built */

    /* insert start node (i=1,j=1) if heap empty */
    if (heap_size == 0) {
        /* value = 47 * 11 * 23 = 11891 */
        seq_t v_start = ((seq_t)47 * 11) * 23;
        heap_push((Node){v_start, 1, 1});
        visited_insert(1, 1);
    }

    /* generate until we have at least target+1 terms */
    while (family_len <= target) {
        Node cur = heap_pop();                 /* smallest value seen so far */
        family_append(cur.val);                 /* add to family cache */

        /* neighbor (i+1, j) */
        if (visited_insert(cur.i + 1, cur.j)) {
            seq_t v1 = cur.val * 11;            /* multiply by base 11 */
            heap_push((Node){v1, cur.i + 1, cur.j});
        }
        /* neighbor (i, j+1) */
        if (visited_insert(cur.i, cur.j + 1)) {
            seq_t v2 = cur.val * 23;            /* multiply by base 23 */
            heap_push((Node){v2, cur.i, cur.j + 1});
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Public function required by the assessment framework                */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n <= 0) return 0;                  /* undefined for n<1, guard */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;

    size_t idx = (size_t)(n - 3);           /* 0‑based index into family */
    ensure_family(idx);
    return family[idx];
}
