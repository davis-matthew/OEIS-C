/*
 * A377411  E.g.f. satisfies A(x) = 1/(1 + A(x)^2 * log(1 - x))^2.
 * https://oeis.org/A377411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Sum_{k=0..n} (5*k+1)!/(4*k+2)! * |Stirling1(n,k)|
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses a dynamic table of Stirling numbers of the first kind
    (unsigned) and the factor (5*k+1)!/(4*k+2)! computed directly as a product.
    The sequence values are cached so successive calls are O(1) after the first
    computation for that n.                                              */
#include <stdlib.h>

static seq_t *a_cache = NULL;        /* a(n) values */
static size_t a_cache_size = 0;

static seq_t *prev_row = NULL;       /* Stirling numbers of previous row */
static size_t prev_size = 0;
static seq_t *curr_row = NULL;       /* Stirling numbers of current row  */
static size_t curr_size = 0;

static seq_t *pk_arr = NULL;         /* (5*k+1)!/(4*k+2)! values  */
static size_t pk_size = 0;

/* Ensure that pk_arr contains entries up to index n */
static void ensure_pk(idx_t n)
{
    if (pk_size > 0 && pk_size > n) return;
    size_t new_size = n + 1;
    pk_arr = (seq_t *)realloc(pk_arr, new_size * sizeof(seq_t));
    for (size_t k = pk_size; k <= n; ++k) {
        seq_t prod = 1;
        /* product of integers from 4*k+3 to 5*k+1 (empty product = 1) */
        for (seq_t m = 4 * k + 3; m <= 5 * k + 1; ++m) {
            prod *= m;
        }
        pk_arr[k] = prod;
    }
    pk_size = new_size;
}

/* Ensure that prev_row and curr_row can hold a row of length l */
static void ensure_stirling_rows(size_t l)
{
    if (curr_size < l) {
        curr_row = (seq_t *)realloc(curr_row, l * sizeof(seq_t));
        curr_size = l;
    }
    if (prev_size < l) {
        prev_row = (seq_t *)realloc(prev_row, l * sizeof(seq_t));
        prev_size = l;
    }
}

/* Ensure that cache holds values up to index n */
static void ensure_cache(idx_t n)
{
    if (a_cache_size > 0 && a_cache_size > n) return;
    size_t new_size = n + 1;
    a_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (a_cache_size == 0) {
        a_cache[0] = (seq_t)1;                 /* a(0) = 1 */
        a_cache_size = 1;
    }
    for (idx_t i = a_cache_size; i <= n; ++i) {
        size_t row_len = (size_t)i + 1;
        ensure_stirling_rows(row_len);
        /* Compute Stirling numbers for row i */
        if (i == 0) {
            prev_row[0] = 1;
        } else {
            curr_row[0] = 0;
            for (size_t k = 1; k < row_len - 1; ++k) {
                curr_row[k] = prev_row[k - 1] + (seq_t)(i - 1) * prev_row[k];
            }
            curr_row[row_len - 1] = 1;
        }
        /* Compute a(i) */
        seq_t sum = 0;
        for (size_t k = 1; k < row_len; ++k) {
            seq_t term = pk_arr[k] * curr_row[k];
            term *= (seq_t)2;
            sum += term;
        }
        a_cache[i] = sum;
        /* Prepare for next iteration: swap rows */
        seq_t *tmp = prev_row;
        prev_row = curr_row;
        curr_row = tmp;
        prev_size = row_len;
    }
    a_cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_pk(n);
    ensure_cache(n);
    return a_cache[(size_t)n];
}
