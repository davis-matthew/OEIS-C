/*
 * A298003  Solution b( ) of the complementary equation a(n) = a(1)*b(n-1) -
 *          a(0)*b(n-2) + 2*n, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4,
 *          and (b(n)) is the increasing sequence of positive integers not in
 *          (a(n)). See Comments.
 * https://oeis.org/A298003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A298003 – the b‑sequence
 *
 *  Recurrence for the complementary a‑sequence:
 *      a(0) = 1,  a(1) = 2
 *      b(0) = 3,  b(1) = 4
 *      a(n) = a(1)*b(n-1) – a(0)*b(n-2) + 2*n   (n ≥ 2)
 *
 *  The b‑sequence consists of the positive integers not appearing in the
 *  a‑sequence, listed in increasing order.  The function below returns
 *  the n‑th element of this b‑sequence.
 *
 *  The routine stores the computed values in static tables and expands
 *  them on demand, allowing calls with arbitrary ordering.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t *a_vals = NULL;      /* a(0)…a(k)   */
static seq_t *b_vals = NULL;      /* b(0)…b(k)   */
static bool *used  = NULL;        /* used[x] = true iff x is an a‑value   */
static size_t len  = 0;           /* number of values currently stored   */
static size_t cap  = 0;           /* capacity of a_vals / b_vals         */
static size_t used_cap = 0;       /* capacity of used array              */

/* Ensure that the boolean array used[] can hold index 'val'. */
static void ensure_used_capacity(idx_t val)
{
    if ((size_t)val < used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 64;
    while (new_cap <= (size_t)val) new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) abort();                   /* out of memory */
    /* initialise the new entries to false */
    memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* Ensure that a_vals / b_vals can hold 'needed' entries. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cap) return;
    size_t new_cap = cap ? cap << 1 : 4;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_vals = tmp;
    tmp = (seq_t *)realloc(b_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    b_vals = tmp;
    cap = new_cap;
}

/* Lazy initialisation of static tables. */
static void init_tables(void)
{
    if (len > 0) return;          /* already initialised */

    /* a(0)=1, a(1)=2 */
    ensure_capacity(2);
    a_vals[0] = (seq_t)1;
    a_vals[1] = (seq_t)2;

    /* b(0)=3, b(1)=4 */
    b_vals[0] = (seq_t)3;
    b_vals[1] = (seq_t)4;

    len = 2;
    cap = 2;

    /* used[1] and used[2] are true (a‑values). */
    used = NULL;
    used_cap = 0;
    ensure_used_capacity(2);
    used[(size_t)1] = true;
    used[(size_t)2] = true;
}

/*----------------------------------------------------------------
 *  Public function: returns the nth element of the b‑sequence.
 *----------------------------------------------------------------*/
seq_t a(idx_t n)
{
    init_tables();

    /* Grow the tables until we have computed up to index n. */
    while (len <= n) {
        idx_t k = (idx_t)len;                 /* compute next index */

        /* --- compute a(k) using the recurrence --- */
        seq_t a_k = a_vals[1] * b_vals[k - 1]
                  - a_vals[0] * b_vals[k - 2]
                  + (seq_t)(2 * k);

        /* store a(k) */
        ensure_capacity(len + 1);
        a_vals[len] = a_k;
        ++len;               /* now len == k+1 */

        /* mark a(k) as used */
        if ((idx_t)a_k < SIZE_MAX)
            ensure_used_capacity((idx_t)a_k);
        used[(size_t)a_k] = true;

        /* --- find b(k) --- */
        seq_t candidate = b_vals[k - 1] + (seq_t)1;
        while (true) {
            if (candidate >= (seq_t)SIZE_MAX) abort();   /* safeguard */
            ensure_used_capacity((idx_t)candidate);
            if (!used[(size_t)candidate]) break;        /* free spot */
            ++candidate;
        }
        b_vals[k] = candidate;
    }

    return b_vals[n];
}
