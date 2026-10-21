/*
 * A294306  Irregular triangle read by rows: T(n, m) = total number of times
 *          the different values appear in row n of A280269, where 0 <= m <=
 *          A280274(n).
 * https://oeis.org/A294306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9220, u16 n <= 9220, u32 n <= 9220, u64 n <= 9220,
 *            u128 n <= 9220, big any n
 * Helpers:   enumerate_i
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A294306
 *  Irregular triangle read by rows.
 *  a(idx) = the concatenation of rows
 *   R(n) = list of frequencies of the smallest k such that
 *          every integer i ≤ n, where i divides n^k for some k,
 *          has a minimal exponent
 *          k = max_p ceil(a_p/e_p),
 *          with p^e_p || n and p^a_p | i.
 *  The first 24 terms:
 *   1,1,1,1,1,1,2,1,1,1,3,1,1,1,1,3,1,2,1,3,1,1,1,1 .
 *
 *  This implementation pre‑computes rows on demand and stores
 *  the concatenated sequence in static arrays.  The algorithm is
 *  O(n·d(n)) (d(n)=number of divisors of n), easily fast enough for
 *  n up to about 10000.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/enumerate_i.h"

/* ------------------------------------------------------------------ */
/*  Factor n into distinct prime bases (p_i) and exponents (e_i). A
 *  simple trial division is sufficient for n ≤ 10000.
 */
static size_t
factor_uint64(uint64_t n, uint64_t primes[], uint64_t exponents[])
{
    size_t cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if ((n % p) == 0) {
            uint64_t e = 0;
            while ((n % p) == 0) {
                n /= p;
                ++e;
            }
            primes[cnt]   = p;
            exponents[cnt] = e;
            ++cnt;
        }
    }
    if (n > 1) {
        primes[cnt]   = n;
        exponents[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Compute the counts array for a single row 'row_n'.
 *  The array 'counts' shall hold frequencies for k = 0 .. kmax.
 *  The length (kmax+1) is written to '*out_len'.
 */
static void
row_counts(uint64_t row_n, seq_t **out_counts, size_t *out_len)
{
    /* Special case n==1: only integer 1 with k=0. */
    if (row_n == 1) {
        seq_t *arr = (seq_t *)calloc(1, sizeof(seq_t));
        arr[0]     = (seq_t)1;
        *out_counts = arr;
        *out_len    = 1;
        return;
    }

    uint64_t primes[16], exponents[16];
    size_t   num_primes = factor_uint64(row_n, primes, exponents);

    int  max_a[16];
    int  kmax_est = 0;
    for (size_t i = 0; i < num_primes; ++i) {
        uint64_t p = primes[i];
        /* largest a such that p^a ≤ row_n */
        int a = 0;
        uint64_t v = 1;
        while (v * p <= row_n) {
            v *= p;
            ++a;
        }
        max_a[i] = a;
        int kk = (a + (int)exponents[i] - 1) / (int)exponents[i];
        if (kk > kmax_est) kmax_est = kk;
    }

    size_t len = (size_t)kmax_est + 1;
    seq_t *counts = (seq_t *)calloc(len, sizeof(seq_t));

    enumerate_i(0, 1, 0, row_n,
                primes, exponents, max_a, num_primes, counts);

    *out_counts = counts;
    *out_len    = len;
}

/* ------------------------------------------------------------------ */
/*  Static buffers holding the sequence built so far.  All indices
 *  here are 0‑based for the arrays but the public index n starts at 1.
 */
static idx_t  *cum         = NULL;    /* cumulative term counts, cum[0]=0    */
static seq_t **row_counts_arr = NULL; /* array of pointers to each row      */
static size_t *row_len_arr   = NULL; /* lengths of each row                 */
static size_t  rows_filled   = 0;     /* number of rows currently stored    */
static size_t  rows_cap      = 0;     /* capacity of the above arrays       */

/*  Ensure that we have computed rows up to the sequence index n.  */
static void
ensure_rows_up_to(idx_t n)
{
    if (cum == NULL) {
        cum = (idx_t *)calloc(2, sizeof(idx_t));   /* cum[0] = 0, will grow */
        row_counts_arr = (seq_t **)malloc(2 * sizeof(seq_t *));
        row_len_arr   = (size_t *)malloc(2 * sizeof(size_t));
        rows_filled   = 0;
        rows_cap      = 2;
        cum[0] = 0;
    }
    while (cum[rows_filled] < n) {
        uint64_t next_row = (uint64_t)(rows_filled + 1); /* row numbers start at 1 */
        seq_t *counts;
        size_t len;
        row_counts(next_row, &counts, &len);

        /* enlarge storage if needed */
        if (rows_filled + 1 >= rows_cap) {
            rows_cap = rows_cap ? rows_cap * 2 : 2;
            cum = (idx_t *)realloc(cum, (rows_cap + 1) * sizeof(idx_t));
            row_counts_arr = (seq_t **)realloc(row_counts_arr,
                                                rows_cap * sizeof(seq_t *));
            row_len_arr   = (size_t *)realloc(row_len_arr,
                                                rows_cap * sizeof(size_t));
        }
        row_counts_arr[rows_filled] = counts;
        row_len_arr[rows_filled]   = len;
        cum[rows_filled + 1] = cum[rows_filled] + (idx_t)len;
        ++rows_filled;
    }
}

/* ------------------------------------------------------------------ */
/*  Public function required by the build system. */
seq_t a(idx_t n)
{
    /* The sequence is 1‑based.  n=1 is valid. */
    if (n == 0) return (seq_t)0;  /* undefined, but avoid crash */

    /* Compute rows until the requested index is covered. */
    ensure_rows_up_to(n);

    /* Locate the row that contains the n‑th term. */
    size_t row = 0;
    while (row < rows_filled && cum[row + 1] < n) ++row;

    /* Position inside the row (0‑based). */
    idx_t pos = n - cum[row] - 1;
    return row_counts_arr[row][(size_t)pos];
}
