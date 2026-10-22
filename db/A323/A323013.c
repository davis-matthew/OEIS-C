/*
 * A323013  Form of Zorach additive triangle T(n,k) (see A035312) where each
 *          number is sum of west and northwest numbers, with the additional
 *          condition that the first element T(n,1) is a Fibonacci number.
 * https://oeis.org/A323013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A323013 – “Zorach additive triangle” (see A035312)
   a(n) : reading the triangle T(n,k) by rows.
   Construction:
     T(1,1) = 1
     T(n,1)  = the smallest Fibonacci number not yet used in the triangle
     T(n,k)  = T(n,k‑1) + T(n‑1,k‑1)          (k>1)
   The sequence a(1),a(2),… is obtained by traversing the triangle
   row‑by‑row.  This implementation builds the triangle lazily and caches
   all values so that subsequent calls are O(1).  All arithmetic is
   performed using seq_t; dynamic arrays are reallocated using
   (seq_t*) realloc(...) so that the code is valid in both C11 and
   C++17.  Only standard headers and the utilities listed in the
   question are used. */

#include <stdlib.h>
#include <stdbool.h>

/* --- static memoisation and dynamic arrays --- */
static seq_t *used = NULL;           /* all values that have already appeared */
static size_t used_len = 0, used_cap = 0;

static seq_t *cache = NULL;          /* a(n) values in order */
static size_t cache_len = 0, cache_cap = 0;

static seq_t *fib = NULL;            /* Fibonacci numbers generated so far */
static size_t fib_len = 0, fib_cap = 0;
static size_t fib_idx = 0;           /* index of next candidate in fib array */

static seq_t *prev_row = NULL;       /* previous row values */
static size_t prev_len = 0;

static size_t max_row = 0;           /* rows computed so far */

/* helper: resize used array if necessary, then append v */
static void add_used(seq_t v)
{
    if (used_len == used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 64;
        seq_t *tmp = (seq_t*)realloc(used, new_cap * sizeof(seq_t));
        used = tmp;
        used_cap = new_cap;
    }
    used[used_len++] = v;
}

/* linear search for membership */
static bool has_used(seq_t v)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used[i] == v) return true;
    return false;
}

/* helper: resize cache array if necessary, then append v */
static void add_cache(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

/* Fibonacci generator helpers */
static void init_fib(void)
{
    if (fib_len == 0) {
        fib_cap = 16;
        fib = (seq_t*)malloc(fib_cap * sizeof(seq_t));
        fib[0] = (seq_t)1;
        fib[1] = (seq_t)1;
        fib_len = 2;
    }
}

static seq_t next_fib_val(void)
{
    if (fib_len == fib_cap) {
        fib_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(fib, fib_cap * sizeof(seq_t));
        fib = tmp;
    }
    seq_t next = fib[fib_len - 1] + fib[fib_len - 2];
    fib[fib_len++] = next;
    return next;
}

/* return the smallest Fibonacci number not yet used in the triangle */
static seq_t next_unused_fib(void)
{
    init_fib();
    while (true) {
        if (fib_idx >= fib_len) next_fib_val();
        seq_t cand = fib[fib_idx++];
        if (!has_used(cand)) return cand;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence generator                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1 – behaviour for n<1 is irrelevant */

    /* determine the row index r such that r*(r+1)/2 >= n */
    size_t target = (size_t)n;
    size_t r = 1;
    while (((size_t)r * (r + 1)) / 2 < target) ++r;

    /* generate rows up to r if necessary */
    for (size_t row = max_row + 1; row <= r; ++row) {
        size_t row_len = row;
        seq_t *new_row = (seq_t*)malloc(row_len * sizeof(seq_t));

        /* first element (westmost) */
        seq_t first = next_unused_fib();
        new_row[0] = first;
        add_used(first);
        add_cache(first);

        /* remaining elements */
        for (size_t k = 1; k < row_len; ++k) {
            new_row[k] = new_row[k - 1] + prev_row[k - 1];
            add_used(new_row[k]);
            add_cache(new_row[k]);
        }

        /* slide window: prev_row becomes new_row */
        free(prev_row);
        prev_row = new_row;
        prev_len = row_len;
    }
    max_row = r;

    /* cache now contains all a(1)..a(max_row*(max_row+1)/2) */
    return cache[target - 1];
}
