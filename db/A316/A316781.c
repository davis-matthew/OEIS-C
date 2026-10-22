/*
 * A316781  a(n) is the Sprague-Grundy value of the Node-Kayles game played on
 *          the graph of n diamonds linked at vertices of degree three.
 * https://oeis.org/A316781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the Sprague‑Grundy value of Node‑Kayles on the graph of
 *            n linked diamonds. It is computed recursively from the K(n) and
 *            X(n) variants: a(n) = mex { K(i‑3)⊕K(n‑i‑1) : 1 ≤ i ≤ n+1 ,
 *            K(j‑2)⊕1⊕K(n‑j‑1) : 1 ≤ j ≤ n } K(n) = mex { K(i‑3)⊕X(n‑i‑1) : 1
 *            ≤ i ≤ n+1 , K(j‑2)⊕1⊕X(n‑j‑1) : 1 ≤ j ≤ n+1...
 * Range:     u8 none, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_MEX 4096   /* assume Grundy values never exceed this */

static seq_t *K_arr = NULL;   /* K(n)  (0‑indexed)  */
static seq_t *X_arr = NULL;   /* X(n)  (0‑indexed)  */
static seq_t *a_arr = NULL;   /* a(n)  (1‑indexed for convenience) */
static idx_t   cap   = 0;     /* array capacity (≥ last index + 1)   */
static idx_t   last  = 0;     /* largest n that has been computed   */

/* ------------------------------------------------------------------ */
/*  helpers for negative indices                                       */
/* ------------------------------------------------------------------ */
static inline seq_t getK_int(int64_t idx)
{
    if (idx < 0) return (seq_t)0;
    return K_arr[(idx_t)idx];
}

static inline seq_t getX_int(int64_t idx)
{
    if (idx < 0) return (seq_t)0;
    return X_arr[(idx_t)idx];
}

/* ------------------------------------------------------------------ */
/*  mex of a Boolean array of size MAX_MEX                            */
/* ------------------------------------------------------------------ */
static seq_t mex_from_seen(const bool *seen)
{
    for (size_t m = 0; m < MAX_MEX; ++m)
        if (!seen[m]) return (seq_t)m;
    /* safeguard – should never reach here for this sequence   */
    return (seq_t)MAX_MEX;
}

/* ------------------------------------------------------------------ */
/*  ensure that all arrays have capacity of at least 'needed+1'       */
/* ------------------------------------------------------------------ */
static void ensure_capacity(idx_t needed)
{
    if (needed < cap) return;

    idx_t new_cap = cap ? cap * 2 : 1;
    while (new_cap <= needed) new_cap <<= 1;

    /* realloc each array – cast to seq_t* because C++ forbids implicit */
    seq_t *tmp;
    tmp = (seq_t *)realloc(K_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();  K_arr = tmp;

    tmp = (seq_t *)realloc(X_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();  X_arr = tmp;

    tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();  a_arr = tmp;

    cap = new_cap;

    /* initialise base values when the first allocation occurs    */
    if (cap >= 1) {
        K_arr[0] = (seq_t)2;
        X_arr[0] = (seq_t)2;
        a_arr[0] = (seq_t)0;        /* unused for a(0) */
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for OEIS offset 1 */

    /* Allocate/expand storage as needed */
    ensure_capacity(n);

    /* Compute all values up to n that have not yet been computed */
    for (idx_t idx = last + 1; idx <= n; ++idx)
    {
        bool seen[MAX_MEX] = {0};
        int64_t i, j;
        /* ---- Compute X(idx) ---- */
        for (i = 1; i <= (int64_t)idx + 1; ++i) {
            int64_t a = i - 3;
            int64_t b = (int64_t)idx - i - 1;
            seq_t v = getX_int(a) ^ getX_int(b);
            if (v < (seq_t)MAX_MEX) seen[(size_t)v] = 1;
        }
        for (j = 0; j <= (int64_t)idx + 1; ++j) {
            int64_t a = j - 2;
            int64_t b = (int64_t)idx - j - 1;
            seq_t v = getX_int(a) ^ (seq_t)1 ^ getX_int(b);
            if (v < (seq_t)MAX_MEX) seen[(size_t)v] = 1;
        }
        X_arr[idx] = mex_from_seen(seen);

        /* ---- Compute K(idx) ---- */
        memset(seen, 0, sizeof(seen));
        for (i = 1; i <= (int64_t)idx + 1; ++i) {
            int64_t a = i - 3;
            int64_t b = (int64_t)idx - i - 1;
            seq_t v = getK_int(a) ^ getX_int(b);
            if (v < (seq_t)MAX_MEX) seen[(size_t)v] = 1;
        }
        for (j = 1; j <= (int64_t)idx + 1; ++j) {
            int64_t a = j - 2;
            int64_t b = (int64_t)idx - j - 1;
            seq_t v = getK_int(a) ^ (seq_t)1 ^ getX_int(b);
            if (v < (seq_t)MAX_MEX) seen[(size_t)v] = 1;
        }
        K_arr[idx] = mex_from_seen(seen);

        /* ---- Compute a(idx) ---- */
        memset(seen, 0, sizeof(seen));
        for (i = 1; i <= (int64_t)idx + 1; ++i) {
            int64_t a = i - 3;
            int64_t b = (int64_t)idx - i - 1;
            seq_t v = getK_int(a) ^ getK_int(b);
            if (v < (seq_t)MAX_MEX) seen[(size_t)v] = 1;
        }
        for (j = 1; j <= (int64_t)idx; ++j) {
            int64_t a = j - 2;
            int64_t b = (int64_t)idx - j - 1;
            seq_t v = getK_int(a) ^ (seq_t)1 ^ getK_int(b);
            if (v < (seq_t)MAX_MEX) seen[(size_t)v] = 1;
        }
        a_arr[idx] = mex_from_seen(seen);
    }

    last = n;
    return a_arr[n];
}
