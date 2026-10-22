/*
 * A353238  Perfect powers that are divisible by 3.
 * https://oeis.org/A353238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth smallest integer of the form (3*m)^k, k>=2, m>=1
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 4625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdbool.h>     /* bool */

/* Helper functions from the utility library */
#include "../utils/pow_int.h"   /* seq_t pow_int(seq_t base, idx_t exp) */
#include "../utils/grow_seq.h"  /* void grow_seq(seq_t **arr, size_t *cap, size_t need) */

/* --- heap to produce the numbers in increasing order ------------------- */
#define MAX_EXP 20   /* exponents k ≥ 2 that we generate */

typedef struct {
    seq_t val;      /* (3*m)^k */
    uint64_t base;  /* the base  = 3*m       */
    uint64_t exp;   /* exponent k              */
} HeapNode;

static HeapNode heap[MAX_EXP + 2]; /* 1‑based indices */
static size_t heap_size = 0;

static inline void heap_swap(HeapNode *a, HeapNode *b)
{
    HeapNode tmp = *a; *a = *b; *b = tmp;
}

static void heap_insert(uint64_t base, uint64_t exp)
{
    seq_t v = pow_int((seq_t)base, (idx_t)exp);
    size_t i = ++heap_size;
    heap[i].val = v;
    heap[i].base = base;
    heap[i].exp = exp;
    /* sift up */
    while (i > 1) {
        size_t p = i >> 1;
        if (heap[i].val < heap[p].val) {
            heap_swap(&heap[i], &heap[p]);
            i = p;
        } else break;
    }
}

static HeapNode heap_pop(void)
{
    HeapNode top = heap[1];
    heap[1] = heap[heap_size--];
    size_t i = 1;
    /* sift down */
    while (1) {
        size_t l = i << 1;
        size_t r = l + 1;
        if (l > heap_size) break;
        size_t smallest = l;
        if (r <= heap_size && heap[r].val < heap[l].val) smallest = r;
        if (heap[smallest].val < heap[i].val) {
            heap_swap(&heap[i], &heap[smallest]);
            i = smallest;
        } else break;
    }
    return top;
}

/* --- cache of generated sequence -------------------------------------- */
static seq_t *cache = NULL;    /* array of terms */
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_size = 0;  /* number of terms inserted */

/* --- initialisation ----------------------------------------------- */
static bool initialized = false;
static seq_t last_value;  /* last value stored in cache */

static void init_sequence(void)
{
    /* initialize heap with the first power for each exponent 2..MAX_EXP */
    for (uint64_t e = 2; e <= MAX_EXP; ++e) {
        heap_insert(3, e);   /* base = 3 (i.e. m=1) */
    }
    heap_size = MAX_EXP - 1; /* because we inserted for 2..MAX_EXP */

    /* initialise cache */
    cache = NULL;
    cache_cap = 0;
    cache_size = 0;
    last_value = (seq_t)-1;  /* a value that cannot appear in the sequence */

    initialized = true;
}

/* Helper to generate terms up to n */
static void generate_up_to(idx_t n)
{
    while (cache_size < n) {
        /* get next smallest candidate */
        HeapNode node = heap_pop();

        /* avoid duplicates */
        if (node.val != last_value) {
            grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = node.val;
            last_value = node.val;
        }

        /* push the next base for the same exponent */
        uint64_t next_base = node.base + 3; /* next multiple of 3 */
        heap_insert(next_base, node.exp);
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();

    if (n == 0) return 0;          /* undefined for n=0, but harmless */

    if ((idx_t)cache_size < n) {
        generate_up_to(n);
    }
    return cache[n - 1];
}
