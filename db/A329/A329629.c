/*
 * A329629  Products of distinct odd primes of squarefree index.
 * https://oeis.org/A329629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest product of distinct odd primes whose prime
 *            * indices are squarefree (index 1 is considered squarefree). The
 *            first * term is the empty product, 1
 * Range:     u8 n <= 1, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/get_prime.h"

/* ---------- Helper: test whether a positive integer is squarefree ---------- */
static inline bool is_squarefree(idx_t n)
{
    if (n < 2) return false;                /* 0 and 1 are not used as indices */
    for (idx_t i = 2; i * i <= n; ++i)
    {
        if (n % (i * i) == 0) return false;
    }
    return true;
}

/* ---------- Dynamic array to hold any seq_t values ---------- */
struct seq_array
{
    seq_t *data;
    size_t size;
    size_t cap;
};

static void seq_array_init(struct seq_array *a, size_t init_cap)
{
    a->size = 0;
    a->cap  = init_cap > 0 ? init_cap : 8;
    a->data = (seq_t *)malloc(a->cap * sizeof(seq_t));
}

static void seq_array_grow(struct seq_array *a)
{
    a->cap *= 2;
    a->data = (seq_t *)realloc(a->data, a->cap * sizeof(seq_t));
}

static void seq_array_push(struct seq_array *a, seq_t val)
{
    if (a->size == a->cap) seq_array_grow(a);
    a->data[a->size++] = val;
}

/* ---------- Node for the min‑heap : product and last prime index used ---------- */
struct heap_node
{
    seq_t prod;   /* current product */
    size_t last;  /* index of last prime used (SIZE_MAX for the root) */
};

struct min_heap
{
    struct heap_node *data;
    size_t size;
    size_t cap;
};

static void heap_init(struct min_heap *h, size_t init_cap)
{
    h->size = 0;
    h->cap  = init_cap > 0 ? init_cap : 16;
    h->data = (struct heap_node *)malloc(h->cap * sizeof(struct heap_node));
}

static void heap_grow(struct min_heap *h)
{
    h->cap *= 2;
    h->data = (struct heap_node *)realloc(h->data, h->cap * sizeof(struct heap_node));
}

static void heap_push(struct min_heap *h, struct heap_node node)
{
    if (h->size == h->cap) heap_grow(h);
    size_t i = h->size++;
    while (i > 0)
    {
        size_t p = (i - 1) >> 1;
        if (h->data[p].prod <= node.prod) break;
        h->data[i] = h->data[p];
        i = p;
    }
    h->data[i] = node;
}

static struct heap_node heap_pop(struct min_heap *h)
{
    struct heap_node top = h->data[0];
    struct heap_node last = h->data[--h->size];
    size_t i = 0;
    while (1)
    {
        size_t left = (i << 1) + 1;
        if (left >= h->size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < h->size && h->data[right].prod < h->data[left].prod)
            smallest = right;
        if (last.prod <= h->data[smallest].prod) break;
        h->data[i] = h->data[smallest];
        i = smallest;
    }
    h->data[i] = last;
    return top;
}

/* ---------- Global state ---------- */
static struct seq_array primes_of_sf = {0,0,0};      /* primes with squarefree index (odd primes only) */
static struct min_heap      h           = {0,0,0};
static struct seq_array     cache      = {0,0,0};
static bool initialized = false;

/* Ensure we have at least 'need' primes in primes_of_sf */
static void ensure_primes(size_t need)
{
    static idx_t next_index = 2;   /* start from prime index 2 (prime 3) */
    while (primes_of_sf.size < need)
    {
        /* find next index whose value is squarefree */
        while (true)
        {
            if (is_squarefree(next_index))
            {
                seq_t p = get_prime(next_index);
                seq_array_push(&primes_of_sf, p);
                ++next_index;
                break;
            }
            ++next_index;
        }
    }
}

/* Initialize static structures once */
static void init(void)
{
    seq_array_init(&primes_of_sf, 64);
    seq_array_init(&cache, 128);
    heap_init(&h, 256);

    /* Pre‑populate a reasonable number of primes (14 primes are enough for
       the first 10 000 terms; we ask for a bit more to be safe). */
    ensure_primes(40);

    /* Root: product 1 with no primes used yet */
    struct heap_node root = {1, (size_t)-1};
    heap_push(&h, root);

    initialized = true;
}

/* ---------- The requested sequence function ---------- */
seq_t a(idx_t n)
{
    if (!initialized) init();

    /* Populate cache until we have at least n terms */
    while ((idx_t)cache.size < n)
    {
        /* Get the next smallest product from the heap */
        struct heap_node cur = heap_pop(&h);

        /* Store it as the next sequence value */
        seq_array_push(&cache, cur.prod);

        /* Generate children: multiply by subsequent primes to keep indices increasing */
        for (size_t j = cur.last + 1; j < primes_of_sf.size; ++j)
        {
            seq_t p = primes_of_sf.data[j];
            /* Avoid overflow for fixed‑width seq_t types */
            if (sizeof(seq_t) <= 8)
            {
                seq_t limit = (seq_t)(-1);
                if (cur.prod > limit / p) continue;   /* would overflow */
            }
            struct heap_node child;
            child.prod = cur.prod * p;
            child.last = j;
            heap_push(&h, child);
        }

        /* If the heap is getting too large relative to the cache size,
           extend the list of primes a bit more (for future products). */
        if (h.size > 2 * cache.size && primes_of_sf.size < 200)
            ensure_primes(primes_of_sf.size + 20);
    }

    return cache.data[n - 1];
}
