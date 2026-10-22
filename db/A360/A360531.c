/*
 * A360531  Numbers of the form F(i)-F(j)-1, i>=5, 3<=j<=i-2.
 * https://oeis.org/A360531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360531:  numbers of the form F(i)-F(j)-1,
   with i >= 5, 3 <= j <= i-2,
   sorted increasingly and without duplicates. */

/* It is computed by generating all values in ascending order
   using a min‑heap per fixed j, storing the smallest unprocessed
   value for each j, and advancing it when extracted.  The sequence
   is cached so that repeated calls, even with arbitrary ordering,
   run in O(n) time overall. */

#include <stdlib.h>
#include <stdbool.h>

/* ----------  Fibonacci cache  ------------------------------------- */
static seq_t *fib = NULL;     /* fib[0] ... fib[fib_len-1] */
static idx_t  fib_len = 0;    /* current length of fib */

/* Ensure fib[n] exists (0‑based).  fib_len is idx_t, allocations use size_t. */
static void ensure_fib_upto(idx_t n)
{
    if (n < fib_len) return;
    size_t new_len = (size_t)(fib_len ? fib_len << 1 : 16);
    while (new_len <= (size_t)n) new_len <<= 1;

    seq_t *new_fib = (seq_t *)realloc(fib, new_len * sizeof(seq_t));
    if (!new_fib) exit(1);   /* allocation failure */
    fib = new_fib;
    for (size_t i = fib_len; i < new_len; ++i)
        fib[i] = fib[i-1] + fib[i-2];
    fib_len = (idx_t)new_len;
}

/* ----------  Min‑heap of (value, j, i)  -------------------------- */
typedef struct {
    seq_t val;   /* current value F(i)-F(j)-1 */
    idx_t j;     /* fixed j of the stream */
    idx_t i;     /* current i position in the stream */
} node_t;

static node_t *heap = NULL;
static size_t  heap_sz = 0;
static size_t  heap_cap = 0;
static idx_t   next_j_to_add = 3;  /* smallest j not yet added as a stream */

/* Ensure heap has room for at least 'needed' entries. */
static void ensure_heap_cap(size_t needed)
{
    if (needed <= heap_cap) return;
    size_t new_cap = heap_cap ? heap_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;

    node_t *new_heap = (node_t *)realloc(heap, new_cap * sizeof(node_t));
    if (!new_heap) exit(1);
    heap = new_heap;
    heap_cap = new_cap;
}

/* Push a node onto the heap. */
static void heap_push(node_t n)
{
    ensure_heap_cap(heap_sz + 1);
    size_t i = heap_sz++;
    heap[i] = n;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        node_t tmp = heap[p]; heap[p] = heap[i]; heap[i] = tmp;
        i = p;
    }
}

/* Pop the minimum node from the heap. */
static node_t heap_pop(void)
{
    node_t root = heap[0];
    --heap_sz;
    heap[0] = heap[heap_sz];
    size_t i = 0;
    while (true) {
        size_t l = (i << 1) + 1;
        if (l >= heap_sz) break;
        size_t r = l + 1;
        size_t s = (r < heap_sz && heap[r].val < heap[l].val) ? r : l;
        if (heap[s].val >= heap[i].val) break;
        node_t tmp = heap[s]; heap[s] = heap[i]; heap[i] = tmp;
        i = s;
    }
    return root;
}

/* Add new streams for all j from next_j_to_add up to max_j
   (where max_j = i_max - 2).  Each stream starts at i=j+2. */
static void add_new_streams_up_to(idx_t i_max)
{
    if (i_max < 5) return;
    idx_t max_j = i_max - 2;
    if (max_j < next_j_to_add) return;

    /* Ensure Fibonacci numbers needed for initial values. */
    ensure_fib_upto(max_j + 2);

    for (idx_t j = next_j_to_add; j <= max_j; ++j) {
        idx_t i_start = j + 2;
        seq_t val = fib[i_start] - fib[j] - (seq_t)1;
        heap_push((node_t){val, j, i_start});
    }
    next_j_to_add = max_j + 1;
}

/* ----------  Cache for sequence values  --------------------------- */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static size_t cache_cap = 0;

static void cache_append(seq_t v)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap << 1 : 16;
        seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;
    }
    cache[cache_len++] = v;
}

/* ----------  Global initializer  ----------------------------------- */
static void init_globals(void)
{
    if (fib) return;  /* already initialized */

    /* Fibonacci array initialised with F(0)=0, F(1)=1. */
    fib = (seq_t *)malloc(16 * sizeof(seq_t));
    if (!fib) exit(1);
    fib[0] = 0;
    fib[1] = 1;
    fib_len = 2;

    heap = NULL;
    heap_sz = 0;
    heap_cap = 0;
    next_j_to_add = 3;

    cache = NULL;
    cache_len = 0;
    cache_cap = 0;
}

/* ----------  Main sequence function  ------------------------------ */
seq_t a(idx_t n)   /* n >= 1 */
{
    if (n == 0) return 0;  /* unused by OEIS */

    init_globals();

    /* Return cached value if already computed. */
    if (cache_len >= n) return cache[n-1];

    /* Initialise first streams if heap empty. */
    if (heap_sz == 0) {
        ensure_fib_upto(5);
        add_new_streams_up_to(5);
    }

    seq_t last_val = 0;
    bool have_last = false;

    /* Generate sequence until we have n terms. */
    while (cache_len < n) {
        node_t cur = heap_pop();

        /* Skip duplicates. */
        if (!have_last || cur.val != last_val) {
            cache_append(cur.val);
            last_val = cur.val;
            have_last = true;
        }

        /* Advance the stream from this j. */
        idx_t next_i = cur.i + 1;
        ensure_fib_upto(next_i);
        seq_t next_val = fib[next_i] - fib[cur.j] - (seq_t)1;
        heap_push((node_t){next_val, cur.j, next_i});

        /* Add any new streams that become available. */
        add_new_streams_up_to(next_i);
    }

    return cache[n-1];
}
