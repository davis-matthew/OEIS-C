/*
 * A334038  a(n) = Product_{p<=n, p prime} binomial(n,p).
 * https://oeis.org/A334038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p≤n, p prime} binomial(n,p)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 16, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n=0,1 ⇒ no primes ≤ n, product = 1 */
    if (n < 2)
        return (seq_t)1;

    /* Generate all primes ≤ n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        /* binomial takes idx_t, which is uint64_t, so primes[i] fits */
        seq_t c = binomial(n, primes[i]);
        prod *= c;
    }

    free(primes);
    return prod;
}
