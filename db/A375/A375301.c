/*
 * A375301  Triangle T(n,k) read by rows with row n equal to the unique
 *          permutation pi_n of [1, ..., n] such that k + pi_n(k) is a power
 *          of 2 for 1 <= k <= n.
 * https://oeis.org/A375301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A375301.
   The sequence is read from triangle T(n,k); row n is the unique
   permutation pi_n of [1..n] such that k + pi_n(k) is a power of 2
   for all 1 ≤ k ≤ n. 
   The n-th term is pi_m(r) where m is the row containing position n
   (1-indexed) and r is the column index within that row.  */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/* Helper: compute next power of two strictly greater than x. */
static uint64_t next_power_of_two(uint64_t x)
{
    /* x > 0 is guaranteed by usage. */
    uint64_t p = 1ULL << (64 - __builtin_clzll(x));
    if (p == x)        /* x is already a power of two */
        p <<= 1;       /* need the next one */
    return p;
}

seq_t a(idx_t n)
{
    /* Ensure n >= 1 as per OEIS offset. */
    if (n == 0)
        return (seq_t)0;

    /* Find row index m such that 1 + 2 + ... + m >= n. */
    idx_t m = 1;
    idx_t sum = 1;            /* sum of first m integers */
    while (sum < n) {
        ++m;
        sum += m;
    }

    /* Compute column index r within row m (1-indexed). */
    idx_t prev_sum = sum - m;        /* sum of first (m-1) integers */
    idx_t r = n - prev_sum;          /* column position in row m */

    /* Allocate temporary arrays for the permutation of size m. */
    seq_t *perm = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    unsigned char *used = (unsigned char *)malloc((m + 1) * sizeof(unsigned char));
    if (!perm || !used) {
        /* Fallback: return 0 if allocation fails. */
        if (perm) free(perm);
        if (used) free(used);
        return (seq_t)0;
    }

    /* Initialize used array to zero. */
    for (idx_t i = 1; i <= m; ++i)
        used[i] = 0;

    /* Construct the unique permutation by pairing largest
       unpaired element with its complement to the next power of two. */
    for (idx_t i = m; i >= 1; --i) {
        if (used[i])
            continue;
        uint64_t p = next_power_of_two((uint64_t)i);
        uint64_t k = p - (uint64_t)i;
        /* k is guaranteed to lie in [1, m]. */
        perm[i] = (seq_t)k;
        perm[k] = (seq_t)i;
        used[i] = 1;
        used[k] = 1;
    }

    seq_t result = perm[r];

    free(perm);
    free(used);
    return result;
}
