/*
 * A356285  a(n) = Sum_{k=0..n} binomial(3*n, k) * q(k), where q(k) is the
 *          number of partitions into distinct parts (A000009).
 * https://oeis.org/A356285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n, k) * q(k) where q(k) = number
 *            of partitions of k into distinct parts
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment. */

/* ------------------------------------------------------------------ */
/* allocation helpers that work for both C and C++                     */
/* ------------------------------------------------------------------ */
static seq_t* alloc_seq(idx_t sz) {
#ifdef __cplusplus
    return new seq_t[sz];
#else
    return (seq_t*)malloc(sz * sizeof(seq_t));
#endif
}

static void free_seq(seq_t* p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* ------------------------------------------------------------------ */
/* compute a(n) on demand, without caching other than the local array */
seq_t a(idx_t n) {
    if (n < 0) return (seq_t)0;   /* safety, negative indices not used */

    /* allocate array for q(0..n) */
    idx_t size = n + 1;
    seq_t* q = alloc_seq(size);
    if (!q) return (seq_t)0;      /* allocation failure */

    /* initialize q array to 0, then set q[0] = 1 */
    for (idx_t i = 0; i < size; ++i) q[i] = (seq_t)0;
    q[0] = (seq_t)1;

    /* DP for partitions into distinct parts */
    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t sum = n; sum >= part; --sum) {
            q[sum] += q[sum - part];
        }
    }

    /* compute the sum a(n) */
    seq_t result = (seq_t)0;
    seq_t binom = (seq_t)1;        /* C(3*n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        result += binom * q[k];

        /* update binomial coefficient to C(3*n, k+1) */
        if (k < n) {
            seq_t numer = (seq_t)( (seq_t)(3 * n) - (seq_t)k );
            seq_t denom = (seq_t)(k + 1);
            binom = binom * numer / denom;
        }
    }

    free_seq(q);
    return result;
}
