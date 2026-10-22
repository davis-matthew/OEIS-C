/*
 * A360772  List of distinct numbers that are powers of odd-indexed Fibonacci
 *          numbers or even powers of nonzero even-indexed Fibonacci numbers.
 * https://oeis.org/A360772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth distinct value of the set * { (Fib(2k+1))^m : k≥0,
 *            m≥1 } ∪ * { (Fib(2k))^(2m) : k≥1, m≥1, Fib(2k)≠0 }
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 114, u64 n <= 274,
 *            u128 n <= 615, big any n
 * Helpers:   add_overflow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/add_overflow.h"

/* --------------------------------------------------------------------- */
/*  Overflow‑safe multiplication and addition for the various seq_t      */
/*  sizes. The helpers treat arbitrary‑precision seq_t (no overflow).   */
/* --------------------------------------------------------------------- */
static inline bool mul_overflow(seq_t a, seq_t b, seq_t *res)
{
    /* Small integer types – detect overflow. */
    if (sizeof(seq_t) <= 8) {
        unsigned long long prod;
        if (__builtin_mul_overflow((unsigned long long)a,
                                   (unsigned long long)b,
                                   &prod))
            return true;
        unsigned long long limit = (unsigned long long)((seq_t)-1);
        if (prod > limit) return true;
        *res = (seq_t)prod;
        return false;
    }
    /* Arbitrary‑precision class – overflow impossible. */
    *res = a * b;
    return false;
}

/* --------------------------------------------------------------------- */
/*  Min‑heap of {value, multiplier} for generating powers.              */
/* --------------------------------------------------------------------- */
struct Node { seq_t val; seq_t mult; };

static struct Node *heap = NULL;
static size_t heap_size = 0, heap_cap = 0;

static inline void heap_grow_if_needed(void)
{
    if (heap_size >= heap_cap) {
        size_t new_cap = heap_cap ? (heap_cap << 1) : 64;
        struct Node *tmp = (struct Node *)realloc((void *)heap,
                                                new_cap * sizeof(struct Node));
        if (!tmp) exit(1);
        heap = tmp;
        heap_cap = new_cap;
    }
}

static inline void swap_node(size_t i, size_t j)
{
    struct Node tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

static void heap_push(struct Node n)
{
    heap_grow_if_needed();
    heap[heap_size++] = n;
    size_t i = heap_size - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        swap_node(p, i);
        i = p;
    }
}

static struct Node heap_pop_min(void)
{
    struct Node top = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t s = i;
        if (l < heap_size && heap[l].val < heap[s].val) s = l;
        if (r < heap_size && heap[r].val < heap[s].val) s = r;
        if (s == i) break;
        swap_node(i, s);
        i = s;
    }
    return top;
}

/* --------------------------------------------------------------------- */
/*  Cache for previously computed sequence values.                      */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0, cache_cap = 0;

static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t cap = cache_cap ? (cache_cap << 1) : 64;
    if (cap < new_cap) cap = new_cap;
    seq_t *tmp = (seq_t *)realloc((void *)cache,
                                  cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = cap;
}

/* --------------------------------------------------------------------- */
/*  Build the heap with Fibonacci seed values.                           */
/* --------------------------------------------------------------------- */
static bool initialized = false;

static void init_sequence(void)
{
    /* Limit for Fibonacci indices: enough for thousands of terms.    */
    const idx_t LIMIT = (sizeof(seq_t) <= 8) ? 200 : 2000;

    seq_t f_prev = 1;       /* Fib(1) */
    seq_t f_curr = 1;       /* Fib(2) */
    seq_t f;               /* current Fib(i) */

    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (i == 1) f = f_prev;
        else if (i == 2) f = f_curr;
        else {
            seq_t next;
            if (add_overflow(f_prev, f_curr, &next)) break;
            f = next;
            f_prev = f_curr;
            f_curr = f;
        }

        /* Skip Fib(2)=1 (even index but value 1).                  */
        if (i == 2) continue;

        if (i & 1) {          /* odd index – all powers of Fib(i)   */
            struct Node n;
            n.val  = f;
            n.mult = (f == 1) ? 0 : f;   /* 1^m is always 1 */
            heap_push(n);
        } else {              /* even index – only even powers      */
            seq_t sq;
            if (!mul_overflow(f, f, &sq)) {
                struct Node n;
                n.val  = sq;             /* first even power (f^2)       */
                n.mult = sq;             /* next power = (f^2)^2 = f^4   */
                heap_push(n);
            }
        }
    }
    initialized = true;
}

/* --------------------------------------------------------------------- */
/*  Public interface: return a(n) (1‑based).                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n<1 */

    /* Ensure the cache holds at least n values.                     */
    if (!initialized) init_sequence();

    while (cache_size < (size_t)n) {
        struct Node cur = heap_pop_min();

        /* Append to cache if the value is new.                      */
        if (cache_size == 0 || cur.val != cache[cache_size-1]) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = cur.val;
        }

        /* Generate the next power for this base (if possible).       */
        if (cur.mult != 0) {
            seq_t next;
            if (!mul_overflow(cur.val, cur.mult, &next)) {
                struct Node nxt = { next, cur.mult };
                heap_push(nxt);
            }
        }
    }
    return cache[n-1];
}
