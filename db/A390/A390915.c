/*
 * A390915  Number of elements of the ring of n-adic integers that satisfy y^k
 *          = y for at least one integer k > 1.
 * https://oeis.org/A390915
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product of the distinct primes dividing n; * for even n
 *            the factor 2 is replaced by 3. In other words * a(n) =
 *            prod_{p|n, p>2} p if n is odd, * 3 * prod_{p|n, p>2} p if n is
 *            even
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation relies on the helper function
 *  size_t factorize(uint64_t n, uint64_t primes[], int exps[]);
 *  from "../utils/factorize.h".  */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factorize.h"

seq_t a(idx_t n)
{
    /* n >= 2 by definition of the sequence */
    uint64_t nn = (uint64_t)n;
    seq_t result = 1;

    /* Buffers for factorization.  64 is more than enough
       for distinct prime factors of any 64‑bit number.  */
    uint64_t primes[64];
    int exps[64];
    size_t cnt = factorize(nn, primes, exps);

    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2ULL)
            result *= (seq_t)3;          /* replace 2 with 3 once */
        else
            result *= (seq_t)primes[i];   /* include each odd prime once */
    }

    return result;
}
