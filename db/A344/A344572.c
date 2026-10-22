/*
 * A344572  a(n) = Sum_{d|n} Omega(d!).
 * https://oeis.org/A344572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} Omega(d!) * Omega(d!) = Σ_{k=2..d} Omega(k)
 *            (Omega = total number of prime factors, counting multiplicity)
 * Range:     u8 n <= 59, u16 n <= 6299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bigomega.h"   /* provides seq_t bigomega(idx_t) */

static seq_t *omegaFact = NULL;     /* cache of Ω(d!) for d = 0..current maximum */
static size_t omegaFactSize = 0;    /* number of elements stored in omegaFact */

/* Return Ω(d!); compute and extend the cache if necessary. */
static seq_t omega_fact(idx_t d)
{
    if ((size_t)d + 1 > omegaFactSize) {
        size_t newSize = (size_t)d + 1;
        seq_t *newArr = (seq_t *)realloc(omegaFact, newSize * sizeof(seq_t));
        if (!newArr)
            abort();   /* out of memory – unlikely in test harness */

        /* Compute newly added entries starting from old size */
        for (size_t i = omegaFactSize; i < newSize; ++i) {
            if (i < 2) {
                newArr[i] = 0;
            } else {
                seq_t prev = newArr[i - 1];
                seq_t add = bigomega((idx_t)i);   /* Ω(i) */
                newArr[i] = prev + add;            /* Ω(i!) = Ω((i-1)!) + Ω(i) */
            }
        }
        omegaFact = newArr;
        omegaFactSize = newSize;
    }
    return omegaFact[d];
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset starts at 1; safeguard */

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            sum += omega_fact(d);
            if (d != n / d)
                sum += omega_fact(n / d);
        }
    }
    return sum;
}
