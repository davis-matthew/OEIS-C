/*
 * A385295  Numbers whose digits all belong to the same residue class mod 6.
 * https://oeis.org/A385295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 171, u32 n <= 2734, u64 n <= 7277,
 *            u128 n <= 7277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all non‑negative integers whose decimal
   digits all belong to the same residue class modulo 6.
   The implementation generates the numbers in increasing order using
   a min‑heap and caches them so that any call `a(n)` can be answered
   in O(1) after an initial O(n log n) build. */

#include <stdlib.h>  /* for malloc, realloc, abort, size_t */
#include <stdint.h>  /* for uint64_t */

/* ------------------------------------------------------------------ */
/* (1) Digit sets for each residue class modulo 6. */
static const int digits[6][2] = {
    {0, 6}, /* residue 0 */
    {1, 7}, /* residue 1 */
    {2, 8}, /* residue 2 */
    {3, 9}, /* residue 3 */
    {4},    /* residue 4 */
    {5}     /* residue 5 */
};
static const int digits_len[6] = {2, 2, 2, 2, 1, 1};

/* ------------------------------------------------------------------ */
/* (2) Min‑heap for the current frontier of candidates. */
static seq_t *heap_vals = NULL;   /* dynamic array of heap elements   */
static size_t heap_size = 0;      /* current number of elements in heap */
static size_t heap_cap  = 0;      /* allocated capacity               */

/* Ensure the heap array can hold at least `needed` elements. */
static void heap_ensure(size_t needed)
{
    if (heap_cap < needed) {
        size_t new_cap = (heap_cap == 0) ? 16 : heap_cap << 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *p = (seq_t *)realloc(heap_vals, new_cap * sizeof(seq_t));
        if (!p) abort();              /* out of memory */
        heap_vals = p;
        heap_cap  = new_cap;
    }
}

/* Push a value onto the min‑heap. */
static void heap_push(seq_t val)
{
    heap_ensure(heap_size + 1);
    size_t i = heap_size++;
    heap_vals[i] = val;
    /* sift‑up */
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap_vals[parent] <= heap_vals[i]) break;
        seq_t tmp = heap_vals[parent];
        heap_vals[parent] = heap_vals[i];
        heap_vals[i] = tmp;
        i = parent;
    }
}

/* Pop the smallest element from the min‑heap. */
static seq_t heap_pop(void)
{
    if (heap_size == 0) abort();  /* should never happen */
    seq_t val = heap_vals[0];
    heap_vals[0] = heap_vals[--heap_size];
    /* sift‑down */
    size_t i = 0;
    while (1) {
        size_t left  = (i << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t child = (right < heap_size && heap_vals[right] < heap_vals[left])
                       ? right : left;
        if (heap_vals[i] <= heap_vals[child]) break;
        seq_t tmp = heap_vals[i];
        heap_vals[i] = heap_vals[child];
        heap_vals[child] = tmp;
        i = child;
    }
    return val;
}

/* ------------------------------------------------------------------ */
/* (3) Cache of already generated sequence values. */
static seq_t *cache    = NULL;   /* array storing the sequence values   */
static size_t cache_len = 0;     /* number of values currently cached   */
static size_t cache_cap = 0;     /* allocated capacity of the cache */

/* Ensure that the cache contains at least `n` terms (1‑based). */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;   /* already enough */

    /* First time: initialise the heap with the single‑digit numbers 0…9. */
    if (heap_size == 0) {
        for (int d = 0; d < 10; ++d)
            heap_push((seq_t)d);
    }

    /* Grow the cache array if needed. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (cache_cap == 0) ? 128 : cache_cap << 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) abort();
        cache    = p;
        cache_cap = new_cap;
    }

    /* Keep extracting the smallest element from the heap until we’ve
       produced `n` terms. */
    while (cache_len < (size_t)n) {
        seq_t val = heap_pop();    /* next smallest number */
        cache[cache_len++] = val;  /* store it */

        if (val == (seq_t)0) continue;   /* 0 produces no children */

        /* The last digit determines the residue class. */
        int r = (int)(val % (seq_t)10) % 6;

        /* Append every allowed digit of this residue class to generate
           the next numbers in the heap. */
        for (int i = 0; i < digits_len[r]; ++i) {
            seq_t child = val * (seq_t)10 + (seq_t)digits[r][i];
            heap_push(child);
        }
    }
}

/* ------------------------------------------------------------------ */
/* The public function as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* behaviour below offset is arbitrary */
    ensure_cache(n);                 /* guarantees cache_len >= n     */
    return cache[n - 1];             /* zero‑based array indexing     */
}
