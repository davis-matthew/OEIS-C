/*
 * A291971  Triangle read by rows: T(n,k) = 3 * T(n-k,k-1) + T(n-k,k) with
 *          T(0,0) = 1 for 0 <= k <= A003056(n).
 * https://oeis.org/A291971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   T(0,0)=1; T(n,0)=0 for n>0. For 1≤k≤A003056(n)
 *            T(n,k)=3*T(n-k,k-1)+T(n-k,k). A003056(n)=⌊(√(8n+1)−1)/2⌋, the
 *            largest k with k(k+1)/2≤n. a(n) is the triangle read by rows
 * Range:     u8 n <= 52, u16 n <= 201, u32 n <= 1054, u64 n <= 6193,
 *            u128 n <= 9981, big any n
 * Helpers:   get_kmax, grow_pointer_array, grow_uchar_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_kmax.h"
#include "../utils/grow_pointer_array.h"
#include "../utils/grow_uchar_array.h"

#ifdef __cplusplus
/* Allocate and free arrays of seq_t in C++ (ensures proper construction).
   For C we fall back to malloc/free. */
static seq_t *alloc_seq_arr(size_t n)
{
    return new seq_t[n];
}

static void free_seq_arr(seq_t *p)
{
    delete[] p;
}
#else
static seq_t *alloc_seq_arr(size_t n)
{
    void *p = malloc(n * sizeof(seq_t));
    if (!p) abort();
    return (seq_t *)p;
}

static void free_seq_arr(seq_t *p)
{
    free(p);
}
#endif

/* --------------------------------------------------------------- */
/*  Helper for computing A003056(n)                                 */

/* --------------------------------------------------------------- */
/*  Dynamic array utilities for pointer arrays                    */

/* --------------------------------------------------------------- */
/*  Cache of the triangle values and related data.                 */
/* --------------------------------------------------------------- */
static seq_t *cache   = NULL;    /* flattened values          */
static size_t cache_cap = 0;
static size_t cache_len = 0;

static seq_t **rows   = NULL;    /* pointers to each row array */
static size_t rows_cap  = 0;
static size_t rows_cnt  = 0;     /* number of rows computed    */

static uint32_t *kmaxs = NULL; /* kmax for each row           */
static size_t kmax_cap = 0;

/* Append a value to the cache, growing it if needed. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t newcap = (cache_cap == 0) ? 64 : (cache_cap << 1);
        seq_t *newptr = alloc_seq_arr(newcap);
        for (size_t i = 0; i < cache_len; ++i)
            newptr[i] = cache[i];
        free_seq_arr(cache);
        cache = newptr;
        cache_cap = newcap;
    }
    cache[cache_len++] = val;
}

/* --------------------------------------------------------------- */
/*  Public sequence function                                       */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure we have computed at least n+1 values. */
    while (cache_len <= (size_t)n) {
        size_t r = rows_cnt;                       /* index of new row   */
        uint32_t kmax = get_kmax(r);               /* max column        */
        size_t row_len = (size_t)kmax + 1;          /* number of entries */

        /* Allocate this row. */
        seq_t *row = alloc_seq_arr(row_len);

        for (size_t k = 0; k < row_len; ++k) {
            seq_t val;
            if (r == 0 && k == 0) {                /* base case          */
                val = (seq_t)1;
            } else if (k == 0) {                   /* first column       */
                val = (seq_t)0;
            } else {
                size_t prev_r = r - k;
                uint32_t prev_kmax = kmaxs[prev_r];
                seq_t a_val = (k - 1 <= prev_kmax) ? rows[prev_r][k - 1] : (seq_t)0;
                seq_t b_val = (k <= prev_kmax)     ? rows[prev_r][k]     : (seq_t)0;
                val = 3 * a_val + b_val;
            }
            row[k] = val;
            cache_append(val);
        }

        /* Store this row. */
        grow_pointer_array((void ***)&rows, &rows_cap, rows_cnt + 1);
        rows[rows_cnt] = row;

        grow_uchar_array(&kmaxs, &kmax_cap, rows_cnt + 1);
        kmaxs[rows_cnt] = kmax;

        ++rows_cnt;
    }

    return cache[(size_t)n];
}
