/*
 * A325498  Difference sequence of A036668.
 * https://oeis.org/A325498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A036668(n+1) – A036668(n) A036668 is defined as: A(1)=1;
 *            for n>1, A(n) is the least positive integer that is not equal to
 *            2*A(m) or 3*A(m) for any m<n. The difference sequence starts
 *            with a(1)=3, a(2)=1, …
 * Range:     u8 n <= 3009, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* basic utilities */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* cache for A036668 values (A[0] = A(1)) */
static seq_t *A = NULL;
static size_t A_cap = 0;
static size_t A_size = 0;

/* forbidden table: true if i equals 2*A(m) or 3*A(m) */
static bool *forbid = NULL;
static size_t forbid_cap = 0;

/* ------------------------------------------------------------------ */
/* ensure at least 'need' capacity for the A array */
static void ensure_a_capacity(size_t need) {
    if (need <= A_cap) return;
    size_t new_cap = A_cap ? A_cap << 1 : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    A = tmp;
    A_cap = new_cap;
}

/* ensure at least 'need' capacity for the forbid table (indices 0..need) */
static void ensure_forbid_cap(size_t need) {
    if (need < forbid_cap) return;         /* need <= forbid_cap-1 */
    size_t new_cap = forbid_cap ? forbid_cap << 1 : 1;
    while (new_cap <= need) new_cap <<= 1;
    bool *tmp = (bool *)realloc(forbid, new_cap * sizeof(bool));
    if (!tmp) abort();
    /* zero the newly allocated part */
    for (size_t i = forbid_cap; i < new_cap; ++i) tmp[i] = false;
    forbid = tmp;
    forbid_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* compute a(n) */
seq_t a(idx_t n) {
    if (n < 1) abort();            /* the sequence is defined for n>=1 */

    /* initialise A036668 if not yet done */
    if (A_size == 0) {
        ensure_a_capacity(1);
        A[0] = (seq_t)1;           /* A(1) = 1 */
        A_size = 1;
        /* mark 2 and 3 as forbidden before generating the next term */
        ensure_forbid_cap(3);
        forbid[2] = true;
        forbid[3] = true;
    }

    /* generate A036668 terms up to index n (0‑based), i.e. up to A[n] */
    while (A_size <= (size_t)n) {
        seq_t last = A[A_size - 1];
        seq_t candidate = last + (seq_t)1;

        /* find the next admissible candidate */
        for (;;) {
            size_t idx = (size_t)candidate;
            ensure_forbid_cap(idx);
            if (!forbid[idx]) break;
            candidate = (seq_t)(idx + 1);    /* increment safely */
        }

        /* append the new term */
        ensure_a_capacity(A_size + 1);
        A[A_size] = candidate;
        ++A_size;

        /* mark its multiples 2* and 3* as forbidden */
        size_t idx  = (size_t)candidate;
        size_t f2   = idx * 2;
        size_t f3   = idx * 3;
        size_t maxf = f2 > f3 ? f2 : f3;
        ensure_forbid_cap(maxf);
        forbid[f2] = true;
        forbid[f3] = true;
    }

    /* difference a(n) = A[n] – A[n-1] (0‑based indices) */
    return A[(size_t)n] - A[(size_t)n - 1];
}
