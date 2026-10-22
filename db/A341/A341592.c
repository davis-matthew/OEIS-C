/*
 * A341592  Number of squarefree superior divisors of n.
 * https://oeis.org/A341592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 6059, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _factor_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of squarefree superior divisors of n */
/* a(n) = #{ d | d∣n, d squarefree, d ≥ n/d } */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/_factor_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* unused offset, but guard anyway */

    /* Get distinct prime factors of n */
    uint64_t primes[32];
    int pc;
    _factor_distinct_primes(n, primes, &pc);

    seq_t count = 0;
    seq_t n_seq = (seq_t)n;

    /* Enumerate all subsets of the distinct primes.
       For each subset we form the product d (squarefree divisor). */
    uint64_t masks = 1ULL << pc;   /* pc <= 6 for n <= 10000 */
    for (uint64_t m = 0; m < masks; ++m) {
        seq_t d = 1;
        for (int i = 0; i < pc; ++i) {
            if (m & (1ULL << i)) {
                d = d * (seq_t)primes[i];
            }
        }
        /* Check superior condition: d ≥ n/d  ⇔  d*d ≥ n */
        seq_t sq = d * d;
        if (sq >= n_seq) {
            count = count + 1;
        }
    }
    return count;
}
