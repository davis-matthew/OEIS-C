/*
 * A360486  Convolution of A000041 and A000290.
 * https://oeis.org/A360486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} p(k)*(n-k)^2
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation precomputes the partition numbers
   p(k) up to the largest index requested so far and then evaluates
   the convolution directly.  The partition numbers are obtained by
   the standard unrestricted partition dynamic programming:
   for each part size i, add its contribution to all larger indices.
   All arithmetic is performed with the build‑supplied seq_t type,
   so no overflow occurs when the value fits into that type. */

#include <stdlib.h>

/* Cached data for partitions and their weighted sums. */
static idx_t max_cached = 0;
static seq_t *p_arr   = NULL;   /* p(k)          */
static seq_t *P_arr   = NULL;   /* sum_{i=0..k} p(i)          */
static seq_t *PK_arr  = NULL;   /* sum_{i=0..k} p(i)*i      */
static seq_t *PK2_arr = NULL;   /* sum_{i=0..k} p(i)*i^2    */

/* Ensure that partitions and cumulative sums up to n are available. */
static void extend_cache(idx_t n)
{
    if (n <= max_cached)
        return;                           /* already available */

    /* Allocate or resize the arrays. */
    size_t cap = (size_t)(n + 1);
    if (p_arr == NULL) {
        p_arr   = (seq_t *)malloc(cap * sizeof(seq_t));
        P_arr   = (seq_t *)malloc(cap * sizeof(seq_t));
        PK_arr  = (seq_t *)malloc(cap * sizeof(seq_t));
        PK2_arr = (seq_t *)malloc(cap * sizeof(seq_t));
    } else {
        p_arr   = (seq_t *)realloc(p_arr,   cap * sizeof(seq_t));
        P_arr   = (seq_t *)realloc(P_arr,   cap * sizeof(seq_t));
        PK_arr  = (seq_t *)realloc(PK_arr,  cap * sizeof(seq_t));
        PK2_arr = (seq_t *)realloc(PK2_arr, cap * sizeof(seq_t));
    }

    /* Reset the partition array before recomputation. */
    for (idx_t i = 0; i <= n; ++i)
        p_arr[i] = (seq_t)0;
    p_arr[0] = (seq_t)1;                 /* p(0) = 1 */

    /* Unrestricted partition dynamic programming (only additions). */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; ++j)
            p_arr[j] += p_arr[j - i];
    }

    /* Compute cumulative sums for the new data. */
    seq_t cumP   = (seq_t)0;
    seq_t cumPK  = (seq_t)0;
    seq_t cumPK2 = (seq_t)0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t ik = (seq_t)i;
        seq_t pk = p_arr[i];
        cumP   += pk;
        cumPK  += pk * ik;
        cumPK2 += pk * ik * ik;
        P_arr[i]   = cumP;
        PK_arr[i]  = cumPK;
        PK2_arr[i] = cumPK2;
    }

    max_cached = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    extend_cache(n);                      /* make sure data are ready */

    if (n == 0)
        return (seq_t)0;                  /* a(0) = 0 */

    seq_t ns    = (seq_t)n;
    seq_t result = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t nk = ns - (seq_t)k;         /* (n - k) */
        result += p_arr[k] * nk * nk;     /* p(k) * (n-k)^2 */
    }

    return result;
}
