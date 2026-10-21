/*
 * A284272  Sum of coefficients > 1 in the Stern polynomial B(n,x): a(n) =
 *          A275812(A260443(n)).
 * https://oeis.org/A284272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of coefficients > 1 in the Stern polynomial B(n,x) *
 *            Implementation uses recursion and memoisation of the coefficient
 *            arrays. * The polynomial arrays are allocated with malloc in C
 *            and with new[] in * C++ to avoid uninitialised objects when
 *            seq_t is an arbitrary‑precision class
 * Range:     u8 n <= 4690, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t **poly_cache   = NULL;  /* pointers to coefficient arrays */
static size_t *poly_len     = NULL;  /* lengths of the arrays            */
static seq_t *sum_cache     = NULL;  /* sum of coefficients > 1         */
static size_t poly_cap      = 0;     /* current capacity of the caches  */

/* Ensure the caches are large enough for indices up to n. */
static void ensure_cache(idx_t n)
{
    if (!poly_cache) {
        poly_cap   = (size_t)n + 1;
        poly_cache = (seq_t **)malloc(poly_cap * sizeof(seq_t *));
        poly_len   = (size_t *)malloc(poly_cap * sizeof(size_t));
        sum_cache  = (seq_t  *)malloc(poly_cap * sizeof(seq_t));
        for (size_t i = 0; i < poly_cap; ++i) poly_cache[i] = NULL;
    } else if ((size_t)n >= poly_cap) {
        size_t new_cap = (size_t)n + 1;
        poly_cache = (seq_t **)realloc(poly_cache, new_cap * sizeof(seq_t *));
        poly_len   = (size_t *)realloc(poly_len,   new_cap * sizeof(size_t));
        sum_cache  = (seq_t  *)realloc(sum_cache,  new_cap * sizeof(seq_t));
        for (size_t i = poly_cap; i < new_cap; ++i) poly_cache[i] = NULL;
        poly_cap = new_cap;
    }
}

/* Allocate an array of seq_t of given length.
 * Use C++ new[] if compiling as C++.  The array is initially uninitialised;
 * elements will be set before use. */
static seq_t *alloc_seq_arr(size_t len)
{
#ifdef __cplusplus
    return new seq_t[len];
#else
    return (seq_t *)malloc(len * sizeof(seq_t));
#endif
}

/* Build the coefficient array for B(n,x) and store its sum of >
 * coefficients > 1.  The arrays are memoised. */
static void compute_poly(idx_t n)
{
    ensure_cache(n);
    if (poly_cache[n] != NULL) return;   /* already computed   */

    if (n == 0) {
        size_t len = 1;
        seq_t *arr = alloc_seq_arr(len);
        arr[0] = (seq_t)0;
        poly_cache[n] = arr;
        poly_len[n]   = len;
        sum_cache[n]  = 0;
        return;
    }
    if (n == 1) {
        size_t len = 1;
        seq_t *arr = alloc_seq_arr(len);
        arr[0] = (seq_t)1;
        poly_cache[n] = arr;
        poly_len[n]   = len;
        sum_cache[n]  = 0;
        return;
    }

    if ((n & 1) == 0) {  /* even: B(2k) = x * B(k) */
        idx_t k = n >> 1;
        compute_poly(k);
        size_t len_k = poly_len[k];
        size_t new_len = len_k + 1;           // shift by one
        seq_t *res = alloc_seq_arr(new_len);
        res[0] = (seq_t)0;
        for (size_t i = 1; i < new_len; ++i)
            res[i] = poly_cache[k][i - 1];
        poly_cache[n] = res;
        poly_len[n]   = new_len;
    } else {              /* odd:  B(2k+1) = B(k)+B(k+1) */
        idx_t k = n >> 1;
        compute_poly(k);
        compute_poly(k + 1);
        size_t len_k  = poly_len[k];
        size_t len_k1 = poly_len[k + 1];
        size_t new_len = (len_k > len_k1 ? len_k : len_k1);
        seq_t *res = alloc_seq_arr(new_len);
        for (size_t i = 0; i < new_len; ++i)
            res[i] = (seq_t)0;
        for (size_t i = 0; i < len_k; ++i)
            res[i] += poly_cache[k][i];
        for (size_t i = 0; i < len_k1; ++i)
            res[i] += poly_cache[k + 1][i];
        poly_cache[n] = res;
        poly_len[n]   = new_len;
    }

    /* compute the sum of coefficients strictly greater than 1 */
    seq_t sum = 0;
    seq_t one = 1;
    for (size_t i = 0; i < poly_len[n]; ++i) {
        if (poly_cache[n][i] > one)
            sum += poly_cache[n][i];
    }
    sum_cache[n] = sum;
}

/* Public function requested by the exercise. */
seq_t a(idx_t n)
{
    compute_poly(n);
    return sum_cache[n];
}
