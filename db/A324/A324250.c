/*
 * A324250  Sequence a(n) = 3*A002559(n) - 2 determining the principal reduced
 *          indefinite binary quadratic form [1, a(n), -a(n)] for Markoff
 *          triples.
 * https://oeis.org/A324250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*A002559(n) - 2, where A002559(n) are the Markoff
 *            numbers (largest component of a Markov triple) sorted by value.
 *            The code generates Markov triples in ascending order of their
 *            largest component using a min‑heap and caches results for
 *            successive calls
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Representation of a sorted Markov triple (x <= y <= z). */
struct Triple {
    seq_t x;
    seq_t y;
    seq_t z;
};

/* Comparison used for the heap: first by z, then by x, then by y. */
static int cmp_triple(const struct Triple *a, const struct Triple *b)
{
    if (a->z < b->z) return -1;
    if (a->z > b->z) return  1;
    if (a->x < b->x) return -1;
    if (a->x > b->x) return  1;
    if (a->y < b->y) return -1;
    if (a->y > b->y) return  1;
    return 0;
}

/* Swap two triples. */
static void swap_triple(struct Triple *a, struct Triple *b)
{
    struct Triple t = *a; *a = *b; *b = t;
}

/* Min‑heap of triples. */
static struct Triple *heap   = NULL;
static size_t          heap_cap = 0;
static size_t          heap_sz  = 0;

/* Ensure the heap has room for at least one more element. */
static void heap_reserve(void)
{
    if (heap_sz == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 16;
        struct Triple *p = (struct Triple *)realloc(heap, new_cap * sizeof(*heap));
        if (p == NULL) exit(1);
        heap = p;
        heap_cap = new_cap;
    }
}

/* Push a triple onto the min‑heap. */
static void heap_push(struct Triple t)
{
    heap_reserve();
    heap[heap_sz++] = t;
    size_t i = heap_sz - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (cmp_triple(&heap[i], &heap[p]) < 0) {
            swap_triple(&heap[i], &heap[p]);
            i = p;
        } else {
            break;
        }
    }
}

/* Pop the minimum triple from the heap. */
static struct Triple heap_pop(void)
{
    struct Triple r = heap[0];
    heap[0] = heap[--heap_sz];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= heap_sz) break;
        size_t rgt = l + 1;
        size_t min = l;
        if (rgt < heap_sz && cmp_triple(&heap[rgt], &heap[l]) < 0)
            min = rgt;
        if (cmp_triple(&heap[min], &heap[i]) < 0) {
            swap_triple(&heap[i], &heap[min]);
            i = min;
        } else {
            break;
        }
    }
    return r;
}

/* Sort three seq_t values into ascending order and return a Triple. */
static struct Triple make_triple(seq_t a, seq_t b, seq_t c)
{
    struct Triple r = { a, b, c };
    if (r.x > r.y) { seq_t t = r.x; r.x = r.y; r.y = t; }
    if (r.y > r.z) { seq_t t = r.y; r.y = r.z; r.z = t; }
    if (r.x > r.y) { seq_t t = r.x; r.x = r.y; r.y = t; }
    return r;
}

/* Cache of computed a(n) values (1‑based indices stored in 0‑based array). */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;    /* size of allocated array (in elements) */
static size_t cache_len = 0;    /* number of values actually stored */

/* Ensure the cache can hold at least n values. */
static void cache_reserve(idx_t n)
{
    if (cache_len < (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap = new_cap ? new_cap * 2 : 16;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
        if (p == NULL) exit(1);
        cache = p;
        cache_cap = new_cap;
    }
}

/* Generate a(n) values up to index n (inclusive). */
static void generate_upto(idx_t n)
{
    cache_reserve(n);
    /* Initial heap content: the root Markov triple (1,1,1). */
    if (heap_sz == 0) {
        struct Triple root = { (seq_t)1, (seq_t)1, (seq_t)1 };
        heap_push(root);
    }

    while (cache_len < (size_t)n) {
        struct Triple t = heap_pop();          /* smallest z so far */
        cache[cache_len++] = ((seq_t)3 * t.z) - (seq_t)2;  /* store a(n) */

        /* Generate children by mutating the smallest and middle elements. */
        seq_t newx = ((seq_t)3 * t.y * t.z) - t.x;
        struct Triple child_x = make_triple(newx, t.y, t.z);
        heap_push(child_x);

        if (!(t.x == t.y)) {  /* avoid duplicate child when x==y */
            seq_t newy = ((seq_t)3 * t.x * t.z) - t.y;
            struct Triple child_y = make_triple(t.x, newy, t.z);
            heap_push(child_y);
        }
    }
}

/* Public entry point required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety guard; sequence starts at index 1 */
    generate_upto(n);
    return cache[n - 1];
}
