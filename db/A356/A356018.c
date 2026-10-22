/*
 * A356018  a(n) is the number of evil divisors (A001969) of n.
 * https://oeis.org/A356018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of evil divisors of n (divisors with an even *
 *            number of 1-bits in binary). A divisor d of n is evil iff *
 *            popcount(d) is even
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper function to factor a 64-bit integer.  The header and function
 * are provided by the OEIS build environment. */
#include "../utils/factor_u64.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* out-of-range, but safe */

    uint64_t N = (uint64_t)n;

    /* Factor N using the provided routine.  It fills arrays
     * `primes` and `exps` with prime factors and their exponents,
     * and stores the number of distinct primes in `cnt`. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(N, primes, exps, &cnt);

    /* Count total number of divisors: product of (exp+1). */
    size_t divisor_cnt = 1;
    for (size_t i = 0; i < cnt; ++i) divisor_cnt *= (exps[i] + 1);

    /* Allocate array to hold all divisors.  The count is small for
     * N <= 10^4 (at most 128 in this range).  For larger N the same
     * logic works because divisor_cnt <= N. */
    seq_t *divs = (seq_t *)malloc(divisor_cnt * sizeof(seq_t));
    if (divs == NULL) return 0;      /* memory failure */

    size_t cur = 1;        /* divs[0] already holds the divisor 1 */
    divs[0] = (seq_t)1;

    /* Generate all divisors by multiplying by prime powers. */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t old = cur;
        uint64_t pk = 1;
        for (uint64_t k = 1; k <= e; ++k) {
            pk *= p;
            for (size_t j = 0; j < old; ++j) {
                uint64_t v = (uint64_t)divs[j] * pk;
                divs[cur++] = (seq_t)v;
            }
        }
    }

    /* Count evil divisors: those with an even popcount. */
    seq_t ans = 0;
    for (size_t i = 0; i < cur; ++i) {
        uint64_t v = (uint64_t)divs[i];
        if ((__builtin_popcountll(v) & 1) == 0) ++ans;
    }

    free(divs);
    return ans;
}
