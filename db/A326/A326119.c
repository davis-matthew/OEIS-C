/*
 * A326119  a(n) is the absolute value of the alternating sum of the first n
 *          increasing perfect powers (A001597): 1, 1-4, 1-4+8, 1-4+8-9, ...
 * https://oeis.org/A326119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = | Sum_{k=1..n} (-1)^k * A001597(k) | (A001597: 1, 4, 8,
 *            9, 16, 25, 27, 32, 36, 49, ... )
 * Range:     u8 n <= 11, u16 n <= 57, u32 n <= 982, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the increasing sequence of perfect powers
   in sorted order with a binary min‑heap.  While generating the first n
   unique perfect powers it keeps two running sums for the alternating
   signed sum, and stores the absolute value of that sum in a table.
   Subsequent calls to a(n) simply look up the table.  The algorithm
   runs in O(n log n) time and uses O(n) extra memory. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Node stored in the heap */
typedef struct {
    seq_t  val;   /* value of the perfect power */
    uint64_t base;/* base of the power */
    uint8_t  exp; /* current exponent */
} hp_node_t;

/* Heaps and tables ---------------------------------------------------- */
static hp_node_t *heap      = NULL;
static size_t    heap_size  = 0;
static size_t    heap_cap   = 0;

static seq_t *abs_sum_table = NULL;
static size_t abs_cap       = 0;
static size_t abs_len       = 0;   /* number of computed a(k) values */

/* Accumulators for the alternating sign sum */
static seq_t pos_sum     = 0;  /* sum of terms with even index */
static seq_t neg_sum     = 0;  /* sum of terms with odd  index */
static seq_t last_output = 0;  /* last unique perfect power output */

/* Counter of how many unique perfect powers have been processed */
static size_t processed = 0;

/* Next base to push its square into the heap (starts with 2) */
static uint64_t next_base = 2;

/* Helper to grow a heap or a table ----------------------------------- */
static void grow_array(void **ptr, size_t *cap, size_t elem_size)
{
    size_t new_cap = (*cap == 0) ? 128 : (*cap << 1);
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) exit(1); /* allocation error */
    *ptr  = tmp;
    *cap  = new_cap;
}

/* Heap operations ----------------------------------------------------- */
static void heap_push(seq_t val, uint64_t base, uint8_t exp)
{
    if (heap_size == heap_cap) {
        grow_array((void**)&heap, &heap_cap, sizeof(hp_node_t));
    }
    heap[heap_size].val  = val;
    heap[heap_size].base = base;
    heap[heap_size].exp  = exp;
    size_t i = heap_size++;
    /* sift up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        hp_node_t tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

static hp_node_t heap_pop(void)
{
    hp_node_t root = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= heap_size) break;
        size_t r = l + 1;
        size_t s = (r < heap_size && heap[r].val < heap[l].val) ? r : l;
        if (heap[i].val <= heap[s].val) break;
        hp_node_t tmp = heap[i];
        heap[i] = heap[s];
        heap[s] = tmp;
        i = s;
    }
    return root;
}

/* Main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure tables are allocated */
    if (abs_cap == 0) {
        grow_array((void**)&abs_sum_table, &abs_cap, sizeof(seq_t));
        abs_len = 0;
    }

    /* Process first perfect power (1) if not already done */
    if (processed == 0) {
        seq_t val1 = 1;
        last_output = val1;
        processed = 1;
        neg_sum += val1;               /* index 1 is odd (negative sign) */
        seq_t diff = (pos_sum >= neg_sum) ? (pos_sum - neg_sum) : (neg_sum - pos_sum);
        if (abs_len == abs_cap) grow_array((void**)&abs_sum_table, &abs_cap, sizeof(seq_t));
        abs_sum_table[abs_len++] = diff;
    }

    /* Generate further perfect powers up to index n */
    while (processed < n) {
        /* Push the next base square into the heap */
        seq_t base_sq = (seq_t)next_base * next_base;
        heap_push(base_sq, next_base, 2);
        ++next_base;

        /* Pop the smallest value */
        hp_node_t node = heap_pop();

        /* Push the next exponent of this node's base */
        seq_t next_val = node.val * node.base;
        heap_push(next_val, node.base, node.exp + 1);

        /* Treat new unique value */
        if (node.val != last_output) {
            last_output = node.val;
            ++processed;                     /* new index */

            if (processed & 1) { /* odd index -> negative sign */
                neg_sum += node.val;
            } else {                 /* even index -> positive sign */
                pos_sum += node.val;
            }

            seq_t diff = (pos_sum >= neg_sum) ? (pos_sum - neg_sum)
                                               : (neg_sum - pos_sum);
            if (abs_len == abs_cap) grow_array((void**)&abs_sum_table, &abs_cap, sizeof(seq_t));
            abs_sum_table[abs_len++] = diff;
        }
    }

    return abs_sum_table[n - 1];
}
