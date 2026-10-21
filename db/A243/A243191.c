/*
 * A243191  Nonnegative integers represented by the indefinite quadratic form
 *          x^2+7xy-y^2.
 * https://oeis.org/A243191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243191 – numbers represented by x^2 + 7xy – y^2.
 * Computation: enumerate all non‑negative integer pairs (x,y),
 * compute the quadratic form, collect the distinct non‑negative values,
 * sort them, and return the n‑th value (offset 1).  The algorithm
 * stops when at least n values have been found; the search bound is
 * doubled as needed.  The implementation works for both fixed‑width
 * integer seq_t and for arbitrary‑precision seq_t (when compiled in C++17). */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Helpers: integer square roots for 64‑bit unsigned integers          */
/* ------------------------------------------------------------------ */
static inline uint64_t floor_sqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (1ULL << 32);   /* 2^32 > sqrt(2^64−1) */
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static inline uint64_t ceil_sqrt_u64(uint64_t n)
{
    uint64_t f = floor_sqrt_u64(n);
    return ( (__int128)f * f == n ) ? f : f + 1;
}

/* ------------------------------------------------------------------ */
/* Comparison for qsort on seq_t                                       */
/* ------------------------------------------------------------------ */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* ------------------------------------------------------------------ */
/* Global state for memoised construction                              */
/* ------------------------------------------------------------------ */
static seq_t *list      = NULL;   /* sorted list of distinct values   */
static size_t list_len  = 0;      /* number of values in list         */
static uint64_t bound   = 2000000ULL;  /* current search bound            */

/* ------------------------------------------------------------------ */
/* Build the list up to at least n_needed elements                     */
/* ------------------------------------------------------------------ */
static void build_sequence(size_t n_needed)
{
    /* Expand bound until we will obtain enough values.
       Doubling ensures O(n log n) complexity of rebuilding. */
    while (list_len < n_needed) {
        /* free previous data if any */
        if (list) { free(list); list = NULL; list_len = 0; }

        /* allocate a presence bitmap up to current bound */
        uint8_t *present = (uint8_t *)calloc(bound + 1, sizeof(uint8_t));
        if (!present) exit(EXIT_FAILURE);

        /* temporary buffer for collected values */
        size_t temp_cap = 1 << 16;   /* 64 k          */
        seq_t *temp     = (seq_t *)malloc(temp_cap * sizeof(seq_t));
        if (!temp) { free(present); exit(EXIT_FAILURE); }
        size_t temp_len = 0;

        /* Enumerate non‑negative pairs (x,y).  For each y we find the
           minimal x that yields a non‑negative value, then iterate x
           until we exceed the bound. */
        for (idx_t y = 0; ; ++y) {
            uint64_t y2  = (uint64_t)y * (uint64_t)y;
            uint64_t prod = 53ULL * y2;
            uint64_t sqrt_term = ceil_sqrt_u64(prod);
            int64_t diff = (int64_t)sqrt_term - (int64_t)7 * (int64_t)y;
            idx_t x_min = 0;
            if (diff > 0)
                x_min = (idx_t)((diff + 1) >> 1);   /* ceil(diff/2) */

            /* minimal value for this y */
            __int128 val128 = (__int128)x_min * x_min +
                              (__int128)7 * x_min * y -
                              (__int128)y2;
            uint64_t val64 = (uint64_t)val128;
            if (val64 > bound) break;              /* further y only larger */

            /* iterate x until value exceeds bound */
            for (idx_t x = x_min; ; ++x) {
                __int128 tmp = (__int128)x * x +
                               (__int128)7 * x * y -
                               (__int128)y2;
                uint64_t v = (uint64_t)tmp;
                if (v > bound) break;
                if (!present[v]) {
                    present[v] = 1;
                    if (temp_len == temp_cap) {
                        temp_cap = temp_cap << 1;
                        seq_t *newp = (seq_t *)realloc(temp,
                                                       temp_cap * sizeof(seq_t));
                        if (!newp) { free(temp); free(present); exit(EXIT_FAILURE); }
                        temp = newp;
                    }
                    temp[temp_len++] = (seq_t)v;
                }
            }
        }

        /* Store collected values, sorted */
        list = (seq_t *)malloc(temp_len * sizeof(seq_t));
        if (!list) { free(temp); free(present); exit(EXIT_FAILURE); }
        for (size_t i = 0; i < temp_len; ++i) list[i] = temp[i];
        free(temp);
        free(present);
        qsort(list, temp_len, sizeof(seq_t), cmp_seq);
        list_len = temp_len;

        /* If still not enough, increase bound and try again */
        if (list_len < n_needed) {
            bound <<= 1;    /* double the bound */
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public entry point                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* safety for n<1 */
    build_sequence((size_t)n);      /* ensures list_len >= n */
    return list[n - 1];
}
