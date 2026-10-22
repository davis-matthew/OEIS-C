/*
 * A300858  a(n) = A243823(n) - A243822(n).
 * https://oeis.org/A300858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n - φ(n) - d(n) + 1) - 2 * N_in * where N_in counts
 *            numbers m ≤ n that are formed only by the prime * divisors of n,
 *            do not divide n, and are >1. * This directly implements a(n) =
 *            A243823(n) – A243822(n) using * the definition of the two
 *            components
 * Range:     i8 n <= 274, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   count_in, dfs_count_in, divisor_count_u64, factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factorize.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/count_in.h"
#include "../utils/dfs_count_in.h"

seq_t a(idx_t n) {
    /* compute φ(n) */
    seq_t phi = (seq_t)n;
    uint64_t primes[64];
    int exps[64];
    size_t pr_cnt = factorize((uint64_t)n, primes, exps);

    for (size_t i = 0; i < pr_cnt; ++i)
        phi -= phi / (seq_t)primes[i];

    /* compute d(n) */
    seq_t d = (seq_t)divisor_count_u64((uint64_t)n);

    /* total nondivisors in the cototient of n */
    seq_t tot = (seq_t)n - phi - d + 1;

    /* count of nondivisors that divide some n^e (first species) */
    seq_t nin = count_in(n, primes, pr_cnt);

    /* a(n) = tot - 2 * N_in */
    return tot - (nin << 1);
}
