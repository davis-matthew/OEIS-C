/*
 * A360533  a(n) = index of the diagonal of the natural number array, A000027,
 *          that includes prime(n). See Comments.
 * https://oeis.org/A360533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = k – n, where the prime p = prime(n) occurs at position *
 *            (i , j) in the natural number array A000027 * w(i , j) = i +
 *            (i+j–2)(i+j–1)/2. * The diagonal index k – n is given by * a(n)
 *            = s + 2 – 2·(p – T_s), where * s is the largest integer with T_s
 *            < p, * T_s = s(s+1)/2. Computation is perfo...
 * Range:     i8 n <= 4, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses the utility routines:
 * – _prime_limit_for_idx to obtain an upper bound for the n‑th prime,
 * – build_primes_upto to generate primes up to that bound,
 * – floor_sqrt to evaluate floor(sqrt(x)) on arbitrary‑precision values. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Static cache of primes so that repeated calls are fast. */
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;
    static idx_t  prime_limit = 0;

    if (n == 0)              /* offset is 1; callers never request n == 0 */
        return 0;

    /* Ensure we have at least the first n primes. */
    if ((size_t)n > prime_count) {
        idx_t limit = _prime_limit_for_idx(n);
        if (limit <= prime_limit)   /* enlarge only if necessary */
            limit = prime_limit + 1;

        uint64_t *new_list;
        size_t    new_cnt;
        build_primes_upto(limit, &new_list, &new_cnt);

        free(prime_list);
        prime_list  = new_list;
        prime_count = new_cnt;
        prime_limit = limit;
    }

    /* p = prime(n)  (seq_t arithmetic is required for generic build). */
    seq_t p = (seq_t)prime_list[n - 1];

    /* Compute s: largest integer with T_s < p, where T_s = s(s+1)/2.  */
    seq_t inner = (seq_t)8 * p + (seq_t)1;          /* 8p + 1  */
    seq_t t_seq = floor_sqrt(inner);               /* floor(sqrt(8p+1)) */
    idx_t  t = (idx_t)t_seq;

    seq_t s_seq = (t - (seq_t)1) / (seq_t)2;       /* floor((sqrt-1)/2) */
    idx_t  s = (idx_t)s_seq;

    /* Adjust when p is exactly a triangular number. */
    if ((seq_t)t * (seq_t)t == inner)
        if (s > 0) s--;

    /* T_s = s(s+1)/2 .  Use seq_t arithmetic to avoid overflow. */
    seq_t T_s = (seq_t)s * (seq_t)(s + 1) / (seq_t)2;

    /* Row index i = p – T_s (1‑based). */
    seq_t i_seq = p - T_s;
    idx_t i    = (idx_t)i_seq;

    /* Diagonal index d = s + 2 – 2·i.  The result may be negative. */
    seq_t d_seq = (seq_t)s + (seq_t)2 - (seq_t)2 * (seq_t)i;
    return d_seq;
}
