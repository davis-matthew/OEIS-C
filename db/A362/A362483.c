/*
 * A362483  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = n! * Sum_{j=0..floor(n/2)} (k/2)^j *
 *          (2*j+1)^(n-j-1) / (j! * (n-2*j)!).
 * https://oeis.org/A362483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where (r,c) are obtained by reading the square
 *            array * in antidiagonals. T(r,c) = r! * Σ_{j=0}^{⌊r/2⌋} (c/2)^j
 *            * (2j+1)^{r-j-1} * / (j! * (r-2j)!). * * This implementation
 *            uses only integer arithmetic and the supplied * arithmetic
 *            utilities
 * Range:     u8 n <= 24, u16 n <= 33, u32 n <= 63, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   factorial, pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/factorial.h"   /* factorial(idx_t n) returns a seq_t */
#include "../utils/pow_seq.h"    /* pow_seq(seq_t base, int exp) */
#include "../utils/pow2.h"       /* pow2(idx_t exp) returns seq_t */

/* integer square root of a 64‑bit unsigned integer */
static idx_t floor_isqrt_u64(idx_t n) {
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;            /* (2^32)^2 = 2^64 > n for all n<2^64 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* cache of factorials */
static seq_t *fact_cache = NULL;
static size_t fact_size = 0;

static void ensure_factorial(size_t n) {
    if (n < fact_size) return;
    size_t old = fact_size;
    fact_size = n + 1;
    fact_cache = (seq_t *)realloc(fact_cache, fact_size * sizeof(seq_t));
    if (fact_cache == NULL) exit(EXIT_FAILURE);
    if (old == 0) {
        fact_cache[0] = (seq_t)1;
        old = 1;
    }
    for (size_t i = old; i <= n; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
}

/* compute T(r,c) as defined above */
static seq_t compute_T(idx_t r, idx_t c) {
    if (r == 0) return (seq_t)1;
    ensure_factorial(r);          /* factorial(r) and below will be ready */
    idx_t maxj = r >> 1;          /* floor(r/2) */
    seq_t sum = (seq_t)0;
    seq_t fact_r = fact_cache[r];
    for (idx_t j = 0; j <= maxj; ++j) {
        idx_t other = r - 2 * j;              /* r-2j >= 0 */
        seq_t pow_cj = pow_seq((seq_t)c, (int)j);
        idx_t exp1 = r - j - 1;               /* exponent >= 0 for r>0 */
        seq_t pow_2j1 = pow_seq((seq_t)(2 * j + 1), (int)exp1);
        seq_t numerator = fact_r * pow_cj;
        numerator = numerator * pow_2j1;
        seq_t denom = fact_cache[j] * fact_cache[other];
        denom = denom * pow2(j);
        seq_t term = numerator / denom;       /* exact division */
        sum += term;
    }
    return sum;
}

/* public sequence function */
seq_t a(idx_t n) {
    /* map n to antidiagonal index t and offset */
    idx_t m = 8 * n + 1;
    idx_t sqrt_m = floor_isqrt_u64(m);
    idx_t t = (sqrt_m - 1) >> 1;
    idx_t prev = t * (t + 1) / 2;
    idx_t offset = n - prev;
    idx_t r = offset;         /* row index in the array */
    idx_t c = t - offset;     /* column index in the array */
    return compute_T(r, c);
}
