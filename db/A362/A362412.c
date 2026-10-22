/*
 * A362412  The number of prime factors of the square root of the largest
 *          square dividing n, counted with multiplicity.
 * https://oeis.org/A362412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001222(A000188(n)) a(n) = number of prime factors of
 *            sqrt(largest square dividing n), counted with multiplicity
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_spf.h"

seq_t a(idx_t n) {
    /* Build (or rebuild) a smallest‑prime‑factor sieve up to n. */
    static idx_t *spf = NULL;      /* array indexed from 0..spf_max */
    static idx_t spf_max = 0;      /* maximum n for which spf is valid */

    if (n < 1) return 0;

    if (n > spf_max) {
        if (spf != NULL) free(spf);
        build_spf(n, &spf);          /* provided utility: idx_t **spf_out */
        spf_max = n;
    }

    idx_t m = n;
    seq_t total = 0;
    while (m > 1) {
        idx_t p = spf[m];
        seq_t exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }
        total += exp / 2;            /* floor division */
    }
    return total;
}
