/*
 * A357379  a(n) = A357378(floor(n/2)) * A357378(n).
 * https://oeis.org/A357379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A357378(floor(n/2)) * A357378(n)
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A357378(n) is defined recursively: 
 *    A357378(0) = 1
 *    For n > 0, let k = floor(n/2) and 
 *    A357378(n) = the smallest positive integer c
 *    such that c * A357378(k) has not yet appeared as a product
 *    of two earlier A357378 values.  
 *  The sequence a(n) is then the product of A357378(n) and 
 *    A357378(floor(n/2)).  The implementation below builds 
 *  A357378 incrementally while keeping a set of previously seen
 *  products in a dynamically sized byte array (used[prod] == 1 if
 *  product prod has already been seen).  Only the product
 *  with floor(n/2) is inserted, which is sufficient for the
 *  recursive definition.
 *
 *  The implementation is fully static, thread‑unsafe, and
 *  works for 32/64/128 bit seq_t as well as arbitrary‑precision
 *  integer types.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

/* ---------- dynamic array for A357378 values ---------- */
static seq_t *t_vals = NULL;
static size_t  t_cap  = 0; /* allocated capacity */
static size_t  t_len  = 0; /* number of elements actually stored */

/* ---------- dynamic byte array for seen products ---------- */
static unsigned char *used = NULL;
static size_t used_cap = 0;

/* ---- helper: ensure t_vals has at least 'cap' entries ---- */
static void ensure_t_capacity(size_t cap)
{
    if (t_cap >= cap) return;
    seq_t *tmp = (seq_t *)realloc(t_vals, cap * sizeof(seq_t));
    if (!tmp) abort();               /* allocation failure */
    t_vals = tmp;
    t_cap  = cap;
}

/* ---- helper: ensure used array has at least 'cap' bytes ---- */
static void ensure_used_capacity(size_t cap)
{
    if (used_cap >= cap) return;
    unsigned char *tmp = (unsigned char *)realloc(used, cap);
    if (!tmp) abort();
    if (cap > used_cap)
        memset(tmp + used_cap, 0, cap - used_cap); /* zero new part */
    used   = tmp;
    used_cap = cap;
}

/* ---- initialise static data on first call ---- */
static void init_static_data(idx_t n_max)
{
    /* small initial allocation that will grow */
    size_t init_cap = 16;
    if (init_cap < (size_t)n_max + 1) init_cap = n_max + 1;
    t_vals = (seq_t *)malloc(init_cap * sizeof(seq_t));
    if (!t_vals) abort();
    t_cap  = init_cap;
    t_vals[0] = (seq_t)1;
    t_len   = 1;                 /* we have 1 element (t[0]) */

    /* used array: mark product 1*1 = 1 as already used */
    used = (unsigned char *)malloc(2);
    if (!used) abort();
    used_cap = 2;
    used[0] = 0;
    used[1] = 1;                 /* product 1 is used */
}

/* ---- augment sequence up to (and including) index 'n' ---- */
static void ensure_up_to(idx_t n)
{
    if (t_vals == NULL)
        init_static_data(n);

    /* increase capacity if needed before filling */
    if ((size_t)n + 1 > t_cap)
        ensure_t_capacity((size_t)n + 1);

    /* fill values from current length up to n */
    for (size_t i = t_len; i <= (size_t)n; ++i) {
        size_t half = i >> 1;                /* floor(i/2) */
        seq_t tk    = t_vals[half];
        seq_t candidate = (seq_t)1;
        seq_t prod;

        /* find smallest candidate such that product not used */
        while (1) {
            prod = candidate * tk;
            size_t idx = (size_t)prod;       /* index into 'used' */
            if (idx >= used_cap)
                ensure_used_capacity(idx + 1);
            if (!used[idx]) break;
            ++candidate;
        }

        t_vals[i] = candidate;              /* store new value */
        size_t idx = (size_t)prod;
        used[idx] = 1;                      /* mark product used */
    }

    t_len = (size_t)n + 1;                /* new boundary of data */
}

/* -------------- public function a(n) -------------- */
seq_t a(idx_t n)
{
    ensure_up_to(n);                       /* guarantees t_vals ready */
    size_t half = (size_t)(n >> 1);
    return t_vals[half] * t_vals[(size_t)n];
}
