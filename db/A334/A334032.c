/*
 * A334032  The a(n)-th composition in standard order (graded
 *          reverse-lexicographic) is the unsorted prime signature of n.
 * https://oeis.org/A334032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the index k of the composition whose parts are the *
 *            exponents in the prime factorisation of n (sorted by ascending
 *            primes). * The mapping from a composition C = (c1,…,cr) to its
 *            index k is: * Let R = (cr,…,c1) (reverse of C). * Let pos = 0. *
 *            For each part p in R (in that order): * pos +...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factorise n, obtaining exponents of the prime factors. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t k = (seq_t)0;
    idx_t pos = 0;                 /* sum of exponents processed so far */

    /* Process the exponents in reverse order (reverse of composition). */
    for (int i = (int)cnt - 1; i >= 0; --i) {
        pos += (idx_t)exps[i];
        k   |= ((seq_t)1) << (pos - 1);   /* set bit at position pos-1 */
    }
    return k;
}
