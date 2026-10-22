/*
 * A328946  Product of primorials of consecutive integers (second definition
 *          A034386).
 * https://oeis.org/A328946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p≤n} p^(n−p+1) with a(0)=a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Handle the trivial cases. */
    if (n <= 1)
        return (seq_t)1;

    /* Generate all primes ≤ n.  The routine allocates an array
       that we must free afterwards. */
    size_t cnt;
    uint64_t *primes = NULL;
    build_primes_upto(n, &primes, &cnt);
    if (!primes) /* allocation failure */
        return (seq_t)0;

    /* Compute the product ∏ p^(n‑p+1).  For each prime p we
       exponentiate p to the exponent e = n+1−p.  The helper
       big_pow works with arbitrary‑precision seq_t values. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        idx_t   exp = (idx_t)(n + 1 - p);   /* exponent ≥ 1 for all primes ≤ n */
        seq_t   term = big_pow((seq_t)p, (seq_t)exp);
        result *= term;
    }

    free(primes);
    return result;
}
