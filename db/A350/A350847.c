/*
 * A350847  Number of even parts in the conjugate of the integer partition
 *          with Heinz number n.
 * https://oeis.org/A350847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even parts in the conjugate of the integer
 *            partition * whose Heinz number is n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses prime factorization of n, conversion from a prime to its index in the
 * sequence of primes, then builds the partition λ from the indices, takes its
 * conjugate λ′, and counts the even parts of λ′.  All operations are performed
 * with the sequence type seq_t. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* include the utilities that provide the prime factors and prime tables */
#include "../utils/factor_u64.h"          /* factor_u64 */
#include "../utils/build_primes_upto.h"   /* build_primes_upto */

/* -------------------------------------------------------------------------- */
/* static helper: get prime index (1‑based) of a prime number p.  The prime
 * table is extended as necessary.  The table is kept in static storage to
 * avoid repeatedly sieving.  The function returns 0 if p is not a prime,
 * which should never happen for a factorization.                     */
static idx_t prime_index_of(uint64_t p)
{
    /* static cache of primes up to primes_limit */
    static uint64_t *prime_tab = NULL;  /* array of primes */
    static size_t prime_cnt = 0;        /* number of primes in prime_tab */
    static idx_t  primes_limit = 0;     /* upper bound of prime_tab values */

    /* extend the table if p is larger than the current limit */
    if (p > (uint64_t)primes_limit) {
        /* free old table if any */
        free(prime_tab);
        build_primes_upto(p, &prime_tab, &prime_cnt);   /* allocates new table */
        primes_limit = p;
    }

    /* binary search for p in the prime table */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_tab[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < prime_cnt && prime_tab[lo] == p)
        return (idx_t)(lo + 1);     /* indices are 1‑based */
    return 0;  /* p is not a prime – should not occur */
}
/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Special case: n == 1  ->  no parts in the partition */
    if (n == 1)
        return 0;

    /* 1. Factor n.  The factor_u64 routine writes the prime factors in
     * `fpr`, multiplicities in `fex`, and the number of distinct prime
     * factors in `fcnt`.  We use a small fixed buffer (64 entries) which
     * is enough for 64‑bit integers. */
    uint64_t fpr[64], fex[64];
    size_t   fcnt = 0;
    factor_u64(n, fpr, fex, &fcnt);

    if (fcnt == 0)  /* n == 1 would have been caught above, but guard anyway */
        return 0;

    /* 2. Convert each prime factor to its prime index.  While doing so we
     * keep track of the maximum index so we can allocate the frequency
     * array.  We record the indices in a temporary array. */
    idx_t *idx_arr = (idx_t *)malloc(fcnt * sizeof(idx_t));
    if (!idx_arr)
        return 0;   /* out of memory – return zero as a safe fallback */

    idx_t  max_idx = 0;
    for (size_t i = 0; i < fcnt; ++i) {
        idx_t idx = prime_index_of(fpr[i]);
        idx_arr[i] = idx;
        if (idx > max_idx)
            max_idx = idx;
    }

    /* 3. Build the frequency array for the indices (partition λ).  All
     * indices run from 1 to max_idx inclusive.  `freq[i]` stores how many
     * parts in λ have value i (i.e. how many times the prime index i
     * occurs in the factorization). */
    seq_t *freq = (seq_t *)calloc(max_idx + 1, sizeof(seq_t));
    if (!freq) {
        free(idx_arr);
        return 0;
    }
    for (size_t i = 0; i < fcnt; ++i) {
        freq[idx_arr[i]] += (seq_t)fex[i];
    }
    free(idx_arr);

    /* 4. Compute the conjugate partition λ′ and count the even parts.
     * λ′_r = sum_{i >= r} freq[i], for r = 1 .. max_idx.
     * We walk r downwards and keep a running cumulative sum.  Only
     * positive sums contribute to the partition parts. */
    seq_t count_even = 0;
    seq_t cum       = 0;
    for (idx_t r = max_idx; r > 0; --r) {
        cum += freq[r];
        if (cum && ((cum & 1) == 0))
            ++count_even;
    }

    free(freq);
    return count_even;
}
