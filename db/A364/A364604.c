/*
 * A364604  Lexicographically earliest sequence, where after every appearance,
 *          a term is banned from re-appearing for twice as long as last time;
 *          first appearance bans it for 1 term.
 * https://oeis.org/A364604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A364604:
   Lexicographically earliest sequence where after each appearance a
   term is banned from re‑appearing for twice as long as the last time;
   first appearance bans it for one term.
   The function builds the sequence incrementally, storing generated
   values in a cache so that any n ≥ 1 can be queried in any order.
   Cached values are kept in seq_cache (0‑based), and each distinct
   sequence value has its current ban length stored in ban_len.  A
   min‑heap (allowed) holds all values currently allowed to appear,
   while a second heap (wait) holds values that will become allowed
   at a future position.  All storage uses dynamic arrays with explicit
   casts from malloc/realloc for C++ compatibility.  The algorithm
   runs in O(n) time and uses O(n) memory, easily meeting the 2 s
   requirement for n up to about 10 000. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper data structures --------------------------------------------------- */

struct AllowedHeap {
    size_t size, cap;
    unsigned int *data;
};

static void allowed_init(struct AllowedHeap *h)
{
    h->size = 0;
    h->cap  = 8;
    h->data = (unsigned int *)malloc(h->cap * sizeof(unsigned int));
}

static void allowed_grow(struct AllowedHeap *h)
{
    if (h->size >= h->cap) {
        h->cap <<= 1;
        h->data = (unsigned int *)realloc(h->data, h->cap * sizeof(unsigned int));
    }
}

static void allowed_push(struct AllowedHeap *h, unsigned int v)
{
    allowed_grow(h);
    size_t i = h->size++;
    h->data[i] = v;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h->data[p] <= h->data[i]) break;
        unsigned int t = h->data[p];
        h->data[p] = h->data[i];
        h->data[i] = t;
        i = p;
    }
}

static unsigned int allowed_pop(struct AllowedHeap *h)
{
    unsigned int res = h->data[0];
    h->data[0] = h->data[--h->size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= h->size) break;
        size_t r = l + 1;
        size_t j = l;
        if (r < h->size && h->data[r] < h->data[l]) j = r;
        if (h->data[i] <= h->data[j]) break;
        unsigned int t = h->data[i];
        h->data[i] = h->data[j];
        h->data[j] = t;
        i = j;
    }
    return res;
}

static bool allowed_empty(const struct AllowedHeap *h) { return h->size == 0; }

/* Heap for values that will become allowed at a future position */

struct WaitNode {
    idx_t next;
    unsigned int val;
};

struct WaitHeap {
    size_t size, cap;
    struct WaitNode *data;
};

static void wait_init(struct WaitHeap *h)
{
    h->size = 0;
    h->cap  = 8;
    h->data = (struct WaitNode *)malloc(h->cap * sizeof(struct WaitNode));
}

static void wait_grow(struct WaitHeap *h)
{
    if (h->size >= h->cap) {
        h->cap <<= 1;
        h->data = (struct WaitNode *)realloc(h->data, h->cap * sizeof(struct WaitNode));
    }
}

static void wait_push(struct WaitHeap *h, idx_t next, unsigned int v)
{
    wait_grow(h);
    size_t i = h->size++;
    h->data[i].next = next;
    h->data[i].val  = v;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        idx_t pn = h->data[p].next;
        idx_t cn = h->data[i].next;
        if (pn < cn || (pn == cn && h->data[p].val <= h->data[i].val))
            break;
        struct WaitNode t = h->data[p];
        h->data[p] = h->data[i];
        h->data[i] = t;
        i = p;
    }
}

static struct WaitNode wait_pop(struct WaitHeap *h)
{
    struct WaitNode res = h->data[0];
    h->data[0] = h->data[--h->size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= h->size) break;
        size_t r = l + 1;
        size_t j = l;
        if (r < h->size &&
            (h->data[r].next < h->data[l].next ||
             (h->data[r].next == h->data[l].next &&
              h->data[r].val < h->data[l].val)))
            j = r;
        if (h->data[i].next < h->data[j].next ||
            (h->data[i].next == h->data[j].next &&
             h->data[i].val <= h->data[j].val))
            break;
        struct WaitNode t = h->data[i];
        h->data[i] = h->data[j];
        h->data[j] = t;
        i = j;
    }
    return res;
}

static bool wait_empty(const struct WaitHeap *h) { return h->size == 0; }

/* Dynamic tables for each distinct value ---------------------------------- */

static size_t val_cap = 0;
static idx_t *ban_len = NULL;   /* current ban length for each value */
static idx_t *next_avail = NULL;/* unused but kept for symmetry */

/* Ensure the tables can hold entries up to value v (inclusive) */
static void value_ensure(unsigned int v)
{
    if (v >= val_cap) {
        size_t newcap = val_cap ? val_cap << 1 : 16;
        while (v >= newcap) newcap <<= 1;
        ban_len = (idx_t *)realloc(ban_len, newcap * sizeof(idx_t));
        next_avail = (idx_t *)realloc(next_avail, newcap * sizeof(idx_t));
        for (size_t i = val_cap; i < newcap; ++i) {
            ban_len[i] = 0;
            next_avail[i] = 0;
        }
        val_cap = newcap;
    }
}

/* Cache for generated sequence terms -------------------------------------- */

static size_t cache_cap = 0;
static seq_t *seq_cache = NULL;

/* Ensure cache can hold n terms (1‑based index) */
static void cache_ensure(idx_t n)
{
    if ((idx_t)cache_cap < n) {
        size_t newcap = cache_cap ? cache_cap << 1 : 16;
        while ((idx_t)newcap < n) newcap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
        for (size_t i = cache_cap; i < newcap; ++i) seq_cache[i] = 0;
        cache_cap = newcap;
    }
}

/* Main sequence function --------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset */

    cache_ensure(n);

    static idx_t generated = 0;    /* terms already generated */
    static int initialized = 0;
    static struct AllowedHeap allowed;
    static struct WaitHeap wait;
    static unsigned int next_new_value = 1;

    if (!initialized) {
        allowed_init(&allowed);
        wait_init(&wait);
        initialized = 1;
    }

    while (generated < n) {
        idx_t pos = ++generated;     /* current 1‑based position */

        /* Move ready values from wait to allowed */
        while (!wait_empty(&wait) && wait.data[0].next <= pos) {
            struct WaitNode wn = wait_pop(&wait);
            allowed_push(&allowed, wn.val);
        }

        unsigned int val;          /* value for current position */

        if (!allowed_empty(&allowed))
            val = allowed_pop(&allowed);
        else
            val = next_new_value++;        /* new unused value */

        value_ensure(val);

        idx_t current_ban = ban_len[val] ? ban_len[val] : 1; /* 1 for first use */
        idx_t next_pos = pos + current_ban + 1;   /* becomes available then */
        ban_len[val] = current_ban * 2;           /* double for future */

        wait_push(&wait, next_pos, val);

        seq_cache[generated - 1] = (seq_t)val;
    }

    return seq_cache[n - 1];
}
