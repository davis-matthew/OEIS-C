/*
 * A343942  Number of even-length strict integer partitions of 2n+1.
 * https://oeis.org/A343942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_distinct(2n+1) + s(2n+1)) / 2 p_distinct(m) – number
 *            of partitions of m into distinct parts s(m) is 0 unless m equals
 *            a generalized pentagonal number g = k(3k±1)/2 (k≥1), then
 *            s(m)=(-1)^k
 * Range:     u8 n <= 13, u16 n <= 38, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   pentagonal_sign
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pentagonal_sign.h"

#if defined(__cplusplus)
/* In C++ use value‑initialising new[]( ) */
static inline seq_t *allocate_array(size_t n) {
    return new seq_t[n]();          /* each element value‑initialised */
}
static inline void free_array(seq_t *p) {
    delete [] p;
}
#else
/* In C use calloc which zeroes the memory */
static inline seq_t *allocate_array(size_t n) {
    return (seq_t*)calloc(n, sizeof(seq_t));
}
static inline void free_array(seq_t *p) {
    free(p);
}
#endif

/* Cache for p_distinct values */
static seq_t *Q          = NULL;      /* Q[m] = p_distinct(m)   */
static size_t Q_cap      = 0;         /* allocated size of Q */

/* Build p_distinct[0 … needed]   (|needed| = max 2n+1)          */
static void compute_Q_up_to(idx_t needed) {
    size_t newCap = (size_t)needed + 1;
    seq_t *newQ = allocate_array(newCap);
    newQ[0] = (seq_t)1;        /* one way to partition 0 */

    for (idx_t k = 1; k <= needed; ++k) {          /* part size k */
        for (idx_t s = needed; s >= k; --s) {       /* sum s */
            newQ[s] += newQ[s - k];
        }
    }

    if (Q) free_array(Q);   /* release old cache if present */
    Q      = newQ;
    Q_cap  = newCap;
}

/* OEIS sequence A343942 – number of even‑length strict integer partitions of 2n+1 */
seq_t a(idx_t n)
{
    idx_t m = 2 * n + 1;                  /* sum to be partitioned */
    if (m >= Q_cap)                       /* enlarge cache if necessary */
        compute_Q_up_to(m);

    seq_t q   = Q[m];                     /* p_distinct(m)   */
    int   s   = pentagonal_sign(m);       /* pentagonal sign */

    if (s > 0)
        q += (seq_t)1;
    else if (s < 0)
        q -= (seq_t)1;

    return q / (seq_t)2;                  /* (p + s)/2  */
}
