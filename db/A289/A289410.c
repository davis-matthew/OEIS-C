/*
 * A289410  Irregular triangular array T(m,k) with m (row) >= 1 and k (column)
 *          >= 1 read by rows: number of m-digit numbers whose digit sum is k.
 * https://oeis.org/A289410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) = T(m,k) where m is the unique integer with 9*(m-1)*m/2 < n
 *            ≤ 9*m*(m+1)/2, and k = n - 9*(m-1)*m/2. T(m,k) counts the
 *            m‑digit numbers with digit sum k (first digit non‑zero)
 * Range:     u8 n <= 64, u16 n <= 205, u32 n <= 495, u64 n <= 495,
 *            u128 n <= 495, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system. */

/* ---- global cache for digit‑sum arrays C[t] --------------------- */
/* C[t] stores counts of t‑digit sequences (digits 0..9) with leading
   zeros allowed.  Index 0 corresponds to t = 0. */
static seq_t **C_arrays = NULL;   /* pointer to C[t] arrays */
static size_t *C_sizes  = NULL;   /* size of each C[t] array (9*t+1) */
static size_t C_cap     = 0;      /* allocated capacity for C_arrays */
static idx_t  C_max     = 0;      /* largest t currently computed (t≥0) */

/* ---- global cache for irregular rows T[m] ---------------------- */
/* T_rows[m] stores the counts for the m‑digit numbers (first digit ≥1). */
static seq_t **T_rows = NULL;     /* pointer to T_rows[m] arrays */
static size_t *T_sizes = NULL;    /* size of each T_rows[m] array (9*m+1) */
static size_t T_cap   = 0;        /* allocated capacity for T_rows */
static idx_t  T_max   = 0;        /* largest m currently computed (m≥1) */

/* --------------------------------------------------------------- */
/* Ensure that C[t] for 0 ≤ t ≤ m_needed-1 and T_rows[m] for 1 ≤ m ≤ m_needed
   are computed and cached. */
static void ensure_tables(idx_t m_needed)
{
    /* ---- initialise C[0] if this is the first call ---- */
    if (!C_arrays)
    {
        C_cap = 1;
        C_arrays = (seq_t**)calloc(C_cap, sizeof(seq_t*));
        C_sizes  = (size_t*)calloc(C_cap, sizeof(size_t));
        C_arrays[0] = (seq_t*)calloc(1, sizeof(seq_t));
        C_arrays[0][0] = 1;
        C_sizes[0] = 1;
        C_max = 0;
    }

    /* ---- compute C[t] for t = C_max+1 … m_needed-1 ---- */
    for (idx_t t = C_max + 1; t <= m_needed - 1; ++t)
    {
        /* Grow storage if needed */
        if ((size_t)(t + 1) > C_cap)
        {
            size_t new_cap = C_cap ? C_cap * 2 : 16;
            while ((size_t)(t + 1) > new_cap) new_cap *= 2;
            C_arrays = (seq_t**)realloc(C_arrays, new_cap * sizeof(seq_t*));
            C_sizes  = (size_t*)realloc(C_sizes,  new_cap * sizeof(size_t));
            for (size_t i = C_cap; i < new_cap; ++i) C_arrays[i] = NULL;
            C_cap = new_cap;
        }

        size_t len = 9ULL * (size_t)t + 1;      /* sums 0 … 9*t */
        C_arrays[t] = (seq_t*)calloc(len, sizeof(seq_t));
        if (!C_arrays[t]) abort();

        seq_t *prev = C_arrays[t - 1];
        size_t prev_len = C_sizes[t - 1];
        seq_t *cur = C_arrays[t];

        /* Convolution of prev with ten 1's (digits 0‑9) */
        for (size_t k = 0; k < len; ++k)
        {
            seq_t sum = 0;
            for (int d = 0; d <= 9; ++d)
            {
                if (k >= (size_t)d && k - (size_t)d < prev_len)
                    sum += prev[k - (size_t)d];
            }
            cur[k] = sum;
        }
        C_sizes[t] = len;
    }
    C_max = m_needed - 1;

    /* ---- initialise T_rows[1] if this is the first call ---- */
    if (!T_rows)
    {
        T_cap = 1;
        T_rows = (seq_t**)calloc(T_cap, sizeof(seq_t*));
        T_sizes = (size_t*)calloc(T_cap, sizeof(size_t));
        T_rows[1] = NULL;   /* will be created below */
    }

    /* ---- compute T_rows[m] for m = T_max+1 … m_needed ---- */
    for (idx_t m = T_max + 1; m <= m_needed; ++m)
    {
        if ((size_t)(m + 1) > T_cap)
        {
            size_t new_cap = T_cap ? T_cap * 2 : 16;
            while ((size_t)(m + 1) > new_cap) new_cap *= 2;
            T_rows = (seq_t**)realloc(T_rows, new_cap * sizeof(seq_t*));
            T_sizes = (size_t*)realloc(T_sizes, new_cap * sizeof(size_t));
            for (size_t i = T_cap; i < new_cap; ++i) T_rows[i] = NULL;
            T_cap = new_cap;
        }

        size_t len = 9ULL * (size_t)m + 1;      /* columns 0 … 9*m */
        T_rows[m] = (seq_t*)calloc(len, sizeof(seq_t));
        if (!T_rows[m]) abort();

        seq_t *prevC = C_arrays[m - 1];
        size_t prev_len = C_sizes[m - 1];
        seq_t *row = T_rows[m];

        /* Compute T(m,k) = Σ_{f=1..9} C[m-1][k-f] */
        for (size_t k = 1; k < len; ++k)   /* k=0 stays 0 */
        {
            seq_t sum = 0;
            for (int f = 1; f <= 9; ++f)
            {
                if (k >= (size_t)f)
                {
                    size_t idx = k - (size_t)f;
                    if (idx < prev_len)
                        sum += prevC[idx];
                }
            }
            row[k] = sum;
        }
        T_sizes[m] = len;
    }
    T_max = m_needed;
}

/* --------------------------------------------------------------- */
/* Sequence entry a(n). */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but return 0 */

    /* Determine the row m containing the n‑th term.  The cumulative
       count up to row m is N(m) = 9*m*(m+1)/2.  */
    idx_t m = 1;
    while ((9ULL * m * (m + 1)) / 2ULL < n) ++m;

    /* Column k within that row (1‑based). */
    idx_t prev_total = (9ULL * (m - 1) * m) / 2ULL;
    idx_t k = n - prev_total;          /* 1 … 9*m */

    /* Ensure all necessary tables are computed. */
    ensure_tables(m);

    /* Return the requested value. */
    return T_rows[m][k];
}
