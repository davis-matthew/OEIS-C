/*
 * A366931  Least k such that 3*n*k+1 is a prime.
 * https://oeis.org/A366931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that 3*n*k + 1 is prime. Computation:
 *            brute‑force search for k starting at 1, testing primality of
 *            3*n*k+1 with isPrimeGeneric
 * Range:     u8 n <= 27, u16 n <= 2900, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* offset is 1; behaviour below is irrelevant */

    seq_t sn   = (seq_t)n;        /* convert index to seq_t for multiplication */
    seq_t three = (seq_t)3;

    for (idx_t k = 1; ; ++k) {
        seq_t ks  = (seq_t)k;
        seq_t v   = three * sn * ks + (seq_t)1;
        if (isPrimeGeneric(v))
            return (seq_t)k;
    }
}
