/*
 * A362375  a(n) = rad((2n-1)!/(n-1)!).
 * https://oeis.org/A362375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all primes ≤ 2n-1 (the square‑free kernel of
 *            (2n‑1)!/(n‑1)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    /* 2n – 1 is the upper bound for the primes that divide the range n··2n‑1.
       For n ≥ 1, the quantity is always non‑negative. */
    idx_t limit = (n << 1) - 1;          /* 2n‑1 */

    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < prime_cnt; ++i)
        prod = prod * (seq_t)primes[i];

    free(primes);
    return prod;
}
