/*
 * A258203  Natural numbers n that have the property that starting from k = n,
 *          the fixed point of the map k -> floor(tan(k)) is strictly
 *          positive, and the smallest number encountered during the iteration
 *          is also strictly positive.
 * https://oeis.org/A258203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 7774, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A258203:
 * a(n) = the n-th natural number n such that,
 * starting with k = n and iteratively applying
 *     k := floor(tan(k)),
 * the orbit reaches a strictly positive fixed point
 * (currently known to be 1) without ever visiting a
 * non‑positive integer.  This is equivalent to
 * the set of integers whose orbit under floor(tan)
 * ends at 1 with all intermediate values > 0.
 *
 * The implementation builds the sequence on demand.
 * It scans successive integers, testing the orbit
 * until it reaches 1 or leaves the positive region.
 *
 * The routine is monotone with respect to any order of
 * calls: earlier computed elements are memoised and
 * new elements are appended as needed.
 */

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Assume seq_t and idx_t are provided by the build environment.
 * We merely cast idx_t values to seq_t when storing. */

static seq_t *seq = NULL;          /* array of computed terms */
static size_t seq_cap = 0;         /* capacity of seq */
static idx_t  seq_len = 0;         /* number of terms currently stored */
static idx_t  next_candidate = 1;  /* next integer to test */

/* Ensure the sequence array can hold at least enough elements. */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory then terminate */
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* Return floor(tan(x)) for integer x using double arithmetic.
 * The resulting value fits into a signed 64‑bit integer for the
 * range of x we actually need to test. */
static long long floor_tan(long long x)
{
    double v = tan((double)x);
    return (long long)floor(v);
}

/* Test whether m satisfies the defining condition of the sequence. */
static bool qualifies(idx_t m)
{
    long long k = (long long)m;
    long long min_k = k;
    while (1) {
        if (k <= 0) return false;               /* went non‑positive -> reject */

        long long f = floor_tan(k);

        /* Stop if we hit a positive fixed point. */
        if (f == k) return (k > 0);

        if (f < min_k) min_k = f;

        /* If we have reached the known fixed point 1, we are done. */
        if (f == 1) return (min_k > 0);

        k = f;  /* iterate */
    }
}

/* Main function requested by the interface */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* under the offset; behavior not important */

    /* Check whether the requested term is already computed */
    if (seq_len >= n) return seq[n - 1];

    /* Compute terms until we reach index n */
    while (seq_len < n) {
        /* Scan candidate integers, updating next_candidate each time */
        while (!qualifies(next_candidate)) ++next_candidate;

        /* Store the found term */
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return seq[n - 1];
}
