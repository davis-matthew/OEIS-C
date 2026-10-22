/*
 * A346924  Expansion of e.g.f. 1 / (1 + log(1 - x)^5 / 5!).
 * https://oeis.org/A346924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * |Stirling1(k,5)| * a(n-k),
 *            a(0) = 1. | The unsigned Stirling numbers of the first kind
 *            satisfy C(n,k) = C(n-1,k-1) + (n-1)*C(n-1,k)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helpers for combinatorics and memory allocation. */
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ensure_cache.h"

/* Helper macro to index the Stirling table. */
#define C(idx, k) (cbuf[(idx) * 6 + (k)])

seq_t a(idx_t n)
{
    /* Static state for memoisation. */
    static seq_t *a_arr = NULL;          /* a[0..computed]          */
    static size_t a_cap = 0;             /* capacity of a_arr        */
    static seq_t *cbuf = NULL;           /* |Stirling| table (rows n,k) */
    static size_t c_cap = 0;             /* capacity of cbuf (rows * 6) */
    static idx_t computed = 0;           /* highest index computed   */

    /* Initialise for the first call. */
    if (a_cap == 0) {
        ensure_cache((idx_t)1, &a_arr, &a_cap);   /* needs a_arr[0] */
        ensure_cache((idx_t)6, &cbuf, &c_cap);     /* row 0, 1 ... */
        /* Base case of Stirling numbers: C(0,0)=1. */
        cbuf[0 * 6 + 0] = (seq_t)1;
        a_arr[0] = (seq_t)1;
        computed = 0;
    }

    /* If n is already computed, return. */
    if (n <= computed) {
        return a_arr[n];
    }

    /* Ensure the buffer is large enough. */
    idx_t needed_len = n + 1;           /* number of a values needed */
    ensure_cache(needed_len, &a_arr, &a_cap);
    /* Each row has 6 entries (k=0..5).  */
    idx_t needed_csize = needed_len * 6;
    ensure_cache(needed_csize, &cbuf, &c_cap);

    /* Compute Stirling numbers for new rows. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i < 5) {
            /* Only C(i,1..i) are possibly non‑zero. */
            for (idx_t k = 1; k <= i; ++k) {
                C(i, k) = C(i - 1, k - 1) + (seq_t)(i - 1) * C(i - 1, k);
            }
        } else {
            /* i >= 5.  Compute all k=1..5. */
            for (idx_t k = 1; k <= 5; ++k) {
                C(i, k) = C(i - 1, k - 1) + (seq_t)(i - 1) * C(i - 1, k);
            }
        }
    }

    /* Compute the sequence entries up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* Only k >= 5 contribute (C(k,5)=0 for k<5). */
        for (idx_t k = 5; k <= i; ++k) {
            seq_t bin = binomial(i, k);
            seq_t prod = bin * C(k, 5);
            prod = prod * a_arr[i - k];
            sum += prod;
        }
        a_arr[i] = sum;
    }

    computed = n;
    return a_arr[n];
}
