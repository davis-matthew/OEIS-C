/*
 * A378622  Array read by antidiagonals downward where A(n,k) is the n-th term
 *          of the k-th differences of the strict partition numbers A000009.
 * https://oeis.org/A378622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = Σ_{t=0}^{k} (-1)^{k-t} * C(k,t) * p_strict(m+t), where
 *            p_strict(n) is the number of strict partitions of n. The pair
 *            (m,k) is obtained from the antidiagonal traversal of difference
 *            sequences: let d be the smallest integer with (d+1)(d+2)/2 > n,
 *            and offset = n - d(d+1)/2. Then m = d - offse...
 * Range:     i8 n <= 43, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies provided by the build environment. */
#include <stdbool.h>
#include "../utils/binomial.h"

/* Pre–compute strict partition numbers up to a safe limit. */
#define MAX_PART 2000
static seq_t part[MAX_PART + 1];
static bool part_initialized = false;

/* Compute p_strict(0..MAX_PART). */
static void init_partitions(void)
{
    if (part_initialized) {
        return;
    }
    for (idx_t i = 0; i <= MAX_PART; ++i) {
        part[i] = 0;
    }
    part[0] = 1;
    for (idx_t val = 1; val <= MAX_PART; ++val) {
        for (idx_t sum = MAX_PART; sum >= val; --sum) {
            part[sum] = part[sum] + part[sum - val];
        }
    }
    part_initialized = true;
}

/* Return a(n) for n ≥ 0. */
seq_t a(idx_t n)
{
    init_partitions();

    /* Find the antidiagonal index d such that cum_{d-1} ≤ n < cum_d,
       where cum_d = (d+1)(d+2)/2. */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n) {
        ++d;
    }

    /* Compute the offset into the antidiagonal and the corresponding
       partition index (m) and difference order (k). */
    idx_t prev_cum = (d * (d + 1)) / 2; /* sum up to d-1 */
    idx_t offset   = n - prev_cum;      /* 0 … d */
    idx_t m        = d - offset;        /* partition index */
    idx_t k        = offset;            /* difference order */

    /* Compute Δ^k p_strict(m) = Σ_{t=0}^k (-1)^{k-t} C(k,t) p_strict(m+t). */
    seq_t result = 0;
    for (idx_t t = 0; t <= k; ++t) {
        seq_t coeff = binomial(k, t);
        seq_t term  = part[m + t];
        if (((k - t) & 1) == 0) {
            result = result + coeff * term;
        } else {
            result = result - coeff * term;
        }
    }

    return result;
}
