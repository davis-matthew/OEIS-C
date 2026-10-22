/*
 * A373077  Perfect powers that are sandwiched between squarefree numbers.
 * https://oeis.org/A373077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th even perfect power x>0 such that (x-1) and
 *            (x+1) are squarefree. The first 24 terms are: 4, 16, 32, 36,
 *            128, 144, 196, 216, 256, 400, …
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generating the sequence on the fly.  The algorithm keeps a min‑heap
   over all perfect powers up to a fixed LIMIT.  The heap contains one
   entry for each base, storing the next power to be generated.  The
   heap is implemented with a static array, so no dynamic allocation of
   seq_t objects is required.  The cache stores the terms that satisfy
   the squarefree neighbour condition.  All data are static so that an
   arbitrary order of calls works.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

/* ---------- configurable limits ------------------------------------- */

#define LIMIT_VAL       10000000000ULL   /* 1e10 : large enough for ≥10000 terms   */
#define CACHE_MAX       20000            /* storage for the sequence values       */
#define MAX_BASE        100005           /* sqrt(LIMIT_VAL)+5                      */

/* ---------- helper comparisons ------------------------------------- */

/* Compare two seq_t values via the built‑in operators.  Works for all
   supplied seq_t types (built‑in or arbitrary‑precision). */
static inline int seq_cmp(const seq_t *a, const seq_t *b)
{
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* ---------- node for the heap ------------------------------------- */
typedef struct {
    seq_t val;   /* next power of this base */
    idx_t base;  /* integer base of the power */
} Node;

/* ---------- static heap data --------------------------------------- */
static Node heap[MAX_BASE];
static size_t heap_size = 0;       /* number of elements in the heap    */
static bool heap_initialized = false;

/* ---------- static cache for the sequence -------------------------- */
static seq_t cache[CACHE_MAX];
static size_t cache_len = 0;

/* ---------- auxiliary: duplicate detection -------------------------- */
static seq_t last_pop_val;
static bool has_last_pop = false;

/* ---------- heap operations ---------------------------------------- */
static inline void heap_push(seq_t v, idx_t b)
{
    size_t i = ++heap_size;
    while (i > 1) {
        size_t parent = i >> 1;
        if (seq_cmp(&(heap[parent].val), &v) <= 0) break;
        heap[i] = heap[parent];
        i = parent;
    }
    heap[i].val  = v;
    heap[i].base = b;
}

static inline Node heap_pop(void)
{
    Node res = heap[1];
    Node last = heap[heap_size--];
    size_t i = 1;
    while (1) {
        size_t child = i << 1;
        if (child > heap_size) break;
        if (child + 1 <= heap_size &&
            seq_cmp(&(heap[child + 1].val), &(heap[child].val)) < 0)
            ++child;
        if (seq_cmp(&(heap[child].val), &(last.val)) < 0) {
            heap[i] = heap[child];
            i = child;
        } else break;
    }
    heap[i] = last;
    return res;
}

/* ---------- heap initialisation ----------------------------------- */
static void init_heap(void)
{
    if (heap_initialized) return;

    const seq_t LIMIT = (seq_t)LIMIT_VAL;
    idx_t base = 2;
    while (1) {
        seq_t sq = (seq_t)base * (seq_t)base;
        if (sq > LIMIT) break;
        heap_push(sq, base);
        ++base;
    }
    heap_initialized = true;
}

/* ---------- main sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for 0 or negative indices */

    /* ensure heap is built */
    init_heap();

    /* generate terms until we have at least n of them */
    const seq_t LIMIT = (seq_t)LIMIT_VAL;
    while (cache_len < (size_t)n && heap_size > 0) {
        Node cur = heap_pop();

        /* skip duplicate values from different bases */
        if (!has_last_pop || seq_cmp(&cur.val, &last_pop_val) != 0) {
            has_last_pop = true;
            last_pop_val = cur.val;

            /* check the squarefree neighbour condition */
            if (is_squarefree(cur.val - 1) && is_squarefree(cur.val + 1)) {
                if (cache_len < CACHE_MAX) {
                    cache[cache_len++] = cur.val;
                }
            }
        }

        /* generate the next power for the same base, if it stays within LIMIT */
        if (cur.val <= (LIMIT / cur.base)) {
            seq_t next_val = cur.val * (seq_t)cur.base;
            heap_push(next_val, cur.base);
        }
    }

    /* return the answer if we have enough terms, otherwise 0 */
    if ((size_t)n <= cache_len) return cache[n - 1];
    return 0;
}
