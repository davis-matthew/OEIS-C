/*
 * A298872  Solution (b(n)) of the system of 3 complementary equations in
 *          Comments.
 * https://oeis.org/A298872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 337, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A298872 – return b(n) from the complementary‑sequence system.
 * a(0)=1, b(0)=2, c(0)=3.
 * a(n)= least unused positive integer;
 * b(n)= least unused integer ≥ a(n)+b(n‑1);
 * c(n)= a(n)+2·b(n).
 * The table is memoised; the function works for any order of calls
 * and for arbitrary‑precision seq_t. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- internal helpers -------------------------------------------- */
static inline void ensure_used_capacity(idx_t n);
static inline void set_used(idx_t val);
static inline bool is_used(idx_t val);

/* global state for memoisation and used‑set */
static idx_t    last_computed = (idx_t)-1;   /* highest index computed */
static seq_t   *A_tbl = NULL;                /* a(n) values              */
static seq_t   *B_tbl = NULL;                /* b(n) values              */
static size_t  A_cap   = 0, B_cap   = 0;     /* capacities of tables      */
static unsigned char *used_bits = NULL;      /* bitset of used numbers   */
static idx_t   used_bits_cap = 0;            /* number of bits allocated */

/* ------------------------------------------------------------------------ */

static inline void ensure_used_capacity(idx_t n)
{
    if (n < used_bits_cap) return;          /* already large enough */
    idx_t new_cap = used_bits_cap ? used_bits_cap << 1 : 64;
    while (new_cap <= n) new_cap <<= 1;
    size_t new_bytes = (new_cap + 7) >> 3;
    unsigned char *new_buf = (unsigned char *)realloc(used_bits, new_bytes);
    if (!new_buf) abort();                  /* memory exhausted */
    size_t old_bytes = used_bits_cap >> 3;
    if (new_bytes > old_bytes)
        memset(new_buf + old_bytes, 0, new_bytes - old_bytes);
    used_bits = new_buf;
    used_bits_cap = new_cap;
}

static inline void set_used(idx_t val)
{
    if (val >= used_bits_cap) ensure_used_capacity(val);
    used_bits[val >> 3] |= (unsigned char)(1U << (val & 7));
}

static inline bool is_used(idx_t val)
{
    if (val >= used_bits_cap) return false; /* not represented yet → unused */
    return (used_bits[val >> 3] >> (val & 7)) & 1;
}

/* ------------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Expand A and B tables if necessary */
    size_t needed = (size_t)(n + 1);
    if (needed > A_cap) {
        size_t cap = A_cap ? A_cap << 1 : 16;
        while (cap < needed) cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(A_tbl, cap * sizeof(seq_t));
        if (!tmp) abort();
        A_tbl = tmp;  A_cap = cap;
    }
    if (needed > B_cap) {
        size_t cap = B_cap ? B_cap << 1 : 16;
        while (cap < needed) cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(B_tbl, cap * sizeof(seq_t));
        if (!tmp) abort();
        B_tbl = tmp;  B_cap = cap;
    }

    /* Initialise on first call */
    if (last_computed == (idx_t)-1) {
        /* Base values a(0)=1, b(0)=2, c(0)=3 – all counted as used */
        set_used((idx_t)1);
        set_used((idx_t)2);
        set_used((idx_t)3);

        A_tbl[0] = (seq_t)1;
        B_tbl[0] = (seq_t)2;
        last_computed = 0;
    }

    /* Generate terms up to requested index */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* ---- compute a(i) ------------------------------------- */
        idx_t candidate = 1;
        while (is_used(candidate)) ++candidate;
        seq_t a_val = (seq_t)candidate;
        A_tbl[i] = a_val;
        set_used(candidate);

        /* ---- compute b(i) ------------------------------------- */
        seq_t target = a_val + B_tbl[i - 1];
        seq_t b_val = target;
        while (is_used((idx_t)b_val)) ++b_val;
        B_tbl[i] = b_val;
        set_used((idx_t)b_val);

        /* ---- compute c(i) ------------------------------------- */
        seq_t c_val = a_val + (seq_t)2 * b_val;
        set_used((idx_t)c_val);
    }

    last_computed = n;
    return B_tbl[n];
}
