/*
 * A348919  Sum of the middle parts of the partitions of k into 3 parts for
 *          all 0 <= k <= n.
 * https://oeis.org/A348919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=1..n} Sum_{k=1..floor(m/3)}
 *            Sum_{i=k..floor((m-k)/2)} i (OEIS A348919)
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* 
 * Helpers to allow dynamic arrays that work in both C and C++.
 * When compiled as C++ we allocate with new[]/delete[] to ensure object
 * construction/destruction. In C we fall back to malloc/realloc.
 */
#ifdef __cplusplus
#include <utility>     /* for std::move (not used but included for completeness) */
#endif

static seq_t* alloc_seq_array(idx_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}

static seq_t* reallocate_seq_array(seq_t* old, idx_t old_len, idx_t new_len) {
    if (old_len == new_len) return old;
#ifdef __cplusplus
    seq_t* newp = new seq_t[new_len];
    /* copy or move old elements */
    for (idx_t i = 0; i < old_len; ++i) {
        newp[i] = std::move(old[i]); /* std::move is harmless if seq_t is a built‑in type */
    }
    delete[] old;
    return newp;
#else
    return (seq_t*)realloc(old, new_len * sizeof(seq_t));
#endif
}

static void free_seq_array(seq_t* p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache of prefix sums: pref[m] == a(m) */
    static seq_t *pref = NULL;
    static idx_t cached = 0;      /* highest n for which pref is valid */

    /* Ensure initial allocation covering index 0 */
    if (pref == NULL) {
        pref = alloc_seq_array(1);   /* size 1: only pref[0] */
        pref[0] = 0;
        cached = 0;
    }

    /* Resize cache if we need to compute beyond the cached range */
    if (n > cached) {
        /* Expand array to hold indices up to n */
        pref = reallocate_seq_array(pref, cached + 1, n + 1);
        /* Compute values for new indices */
        for (idx_t m = cached + 1; m <= n; ++m) {
            seq_t T = (seq_t)0;                     /* sum over k and i for fixed m   */
            idx_t Kmax = m / 3;                     /* floor(m/3) */
            for (idx_t k = 1; k <= Kmax; ++k) {
                idx_t i_min = k;
                idx_t i_max = (m - k) / 2;          /* floor((m-k)/2) */
                if (i_max < i_min) continue;
                idx_t count = i_max - i_min + 1;
                seq_t sum_i = ((seq_t)i_min + (seq_t)i_max) * (seq_t)count / (seq_t)2;
                T += sum_i;
            }
            pref[m] = pref[m - 1] + T;
        }
        pref[0] = 0;          /* keep a(0)=0 even after reallocation */
        cached = n;
    }

    return pref[n];
}
