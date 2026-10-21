/*
 * A257618  Triangle read by rows: T(n,k) = t(n-k, k); t(n,m) = f(m)*t(n-1,m)
 *          + f(n)*t(n,m-1), where f(x) = 8*x + 2.
 * https://oeis.org/A257618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8n+1)-1)/2), c = n -
 *            r(r+1)/2, and the triangle T is defined by T(0,0) = 1, T(n,0) =
 *            2^n for n > 0, T(n,k) = (8*k+2) * T(n-1,k) + (8*(n-k)+2) *
 *            T(n-1,k-1) for 1 ≤ k ≤ n. The sequence a(n) is obtained by
 *            reading the triangle row‑by‑row
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 38, u64 n <= 95, u128 n <= 261,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Conditional allocation helpers: use new/delete in C++ and malloc/free in C */
#ifdef __cplusplus
# define ALLOC_SEQ_T_ARRAY(ptr, sz) ((ptr) = new seq_t[(sz)])
# define FREE_SEQ_T_ARRAY(ptr)     delete[] (ptr)
# define ALLOC_PTR_ARRAY(ptr, sz) ((ptr) = new seq_t*[(sz)])
# define FREE_PTR_ARRAY(ptr)     delete[] (ptr)
#else
# define ALLOC_SEQ_T_ARRAY(ptr, sz) ((ptr) = (seq_t*)malloc((sz)*sizeof(seq_t)))
# define FREE_SEQ_T_ARRAY(ptr)     free(ptr)
# define ALLOC_PTR_ARRAY(ptr, sz) ((ptr) = (seq_t**)malloc((sz)*sizeof(seq_t*)))
# define FREE_PTR_ARRAY(ptr)     free(ptr)
#endif

seq_t a(idx_t n)
{
    /* 1. Locate the row r and column c of a(n) in the flattened triangle */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t c = n - r * (r + 1) / 2;

    /* 2. Static cache for the triangle */
    static seq_t **T = NULL;     /* array of pointers to rows */
    static idx_t max_row = 0;    /* highest row computed so far */
    static size_t ptr_cap = 0;   /* current capacity of T pointer array */

    /* 3. Initialise the cache on first call */
    if (T == NULL) {
        ptr_cap = 1;
        ALLOC_PTR_ARRAY(T, ptr_cap);
        if (T == NULL) { exit(EXIT_FAILURE); }
        ALLOC_SEQ_T_ARRAY(T[0], 1);
        if (T[0] == NULL) { free(T); exit(EXIT_FAILURE); }
        T[0][0] = (seq_t)1;   /* T(0,0)=1 */
        max_row = 0;
    }

    /* 4. Grow the pointer array to hold row r, if necessary */
    if (ptr_cap < (size_t)(r + 1)) {
        size_t new_cap = r + 1;
        seq_t **new_T;
#ifdef __cplusplus
        new_T = new seq_t*[new_cap];
        for (size_t j = 0; j < ptr_cap; ++j) new_T[j] = T[j];
        delete[] T;
#else
        new_T = (seq_t**)malloc(new_cap * sizeof(seq_t*));
        for (size_t j = 0; j < ptr_cap; ++j) new_T[j] = T[j];
        free(T);
#endif
        T = new_T;
        ptr_cap = new_cap;
    }

    /* 5. Compute any missing rows up to r */
    if (r > max_row) {
        for (idx_t i = max_row + 1; i <= r; ++i) {
            /* Allocate row i with i+1 elements */
            ALLOC_SEQ_T_ARRAY(T[i], i + 1);
            if (T[i] == NULL) {
                /* Allocation failure – clean up and abort */
#ifdef __cplusplus
                for (idx_t j = max_row + 1; j < i; ++j) delete[] T[j];
                delete[] T;
#else
                for (idx_t j = max_row + 1; j < i; ++j) free(T[j]);
                free(T);
#endif
                exit(EXIT_FAILURE);
            }

            /* First element: T(i,0) = 2^i */
            T[i][0] = T[i - 1][0] * (seq_t)2;

            /* Remaining elements */
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term1 = (k <= i - 1) ? T[i - 1][k] : 0;
                seq_t term2 = T[i - 1][k - 1];          /* uses previous row */
                seq_t f_k   = (seq_t)(8 * k + 2);       /* 8k+2 */
                seq_t f_ik  = (seq_t)(8 * (i - k) + 2);  /* 8(i-k)+2 */
                T[i][k] = f_k * term1 + f_ik * term2;
            }
        }
        max_row = r;
    }

    /* 6. Return the requested triangle entry */
    return T[r][c];
}
