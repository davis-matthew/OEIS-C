/*
 * A338610  Integers m such that there exist one prime p and one positive
 *          integer k, for which the expression k^3 + k^2*p is a perfect cube
 *          m^3.
 * https://oeis.org/A338610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A111251(n)+1)*(A111251(n))^2 (i.e. a(n) = ((q+1)*q*q)
 *            where q is the n‑th integer such that 3*q^2+3*q+1 is prime)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 402, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined for n=0 */

    /* cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;       /* allocated capacity */
    static size_t  count = 0;       /* number of terms stored */
    static uint64_t last_q = 0;     /* last q examined */

    while (count < n) {
        uint64_t q = last_q + 1;

        /* p = 3*q^2 + 3*q + 1 */
        __int128 p128 = (__int128)3 * q * q + (__int128)3 * q + 1;
        seq_t p = (seq_t)(uint64_t)p128;          /* p fits in 64 bits */

        if (isPrimeGeneric(p)) {
            /* m = (q+1)*q^2 */
            __int128 qsq = (__int128)q * q;
            seq_t m = (seq_t)(qsq * (q + 1));

            /* grow cache if necessary */
            if (count == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cap = new_cap;
            }
            cache[count++] = m;
        }
        last_q = q;
    }

    return cache[n - 1];
}
