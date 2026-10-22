/*
 * A338463  Expansion of g.f.: (-1 + Product_{k>=1} 1 / (1 + (-x)^k))^2.
 * https://oeis.org/A338463
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 74, u32 n <= 231, u64 n <= 780,
 *            u128 n <= 1002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A338463
   a(n) = Sum_{k=1..n-1} A000700(k) * A000700(n-k)
   where A000700(k) is the number of partitions of k into
   distinct odd parts.  The sequence is defined for n ≥ 2.
   
   The implementation keeps a cache of the partition counts up to
   the largest n requested and recomputes them when a larger n
   arrives.  Each call then evaluates the convolution in O(n)
   time.  This satisfies the performance requirement for n ≤ 10000
   and works with any of the provided integer types. */

#include <stdlib.h>

/* Cached array of A000700 values
   f_arr[i] = number of partitions of i into distinct odd parts   */
static seq_t *f_arr = NULL;
static idx_t  f_cap = 0;   /* highest index computed in f_arr */

/* helper to (re)compute f_arr up to given limit */
static void recompute_f(idx_t limit)
{
    size_t cap = (size_t)(limit + 1);
    seq_t *new_f = (seq_t *)calloc(cap, sizeof(seq_t));
    if (!new_f) return;          /* memory allocation failed – stay with old data */

    new_f[0] = 1;                 /* empty partition */
    for (idx_t odd = 1; odd <= limit; odd += 2) {
        for (idx_t i = limit; i >= odd; --i) {
            new_f[i] += new_f[i - odd];
        }
    }

    free(f_arr);
    f_arr = new_f;
    f_cap  = limit;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* a(2) is the first defined term; lower indices return 0 */
    if (n < 2) return 0;

    /* make sure we have f_arr computed up to n */
    if (n > f_cap) {
        recompute_f(n);
        if (f_arr == NULL) return 0;   /* allocation failed */
    }

    /* compute the convolution for this specific n */
    seq_t result = 0;
    for (idx_t k = 1; k < n; ++k) {
        result += f_arr[k] * f_arr[n - k];
    }
    return result;
}
