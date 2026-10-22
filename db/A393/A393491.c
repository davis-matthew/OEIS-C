/*
 * A393491  Multiplicative sequence a(n) with a(p^e) = floor((e+1)/3) for
 *          prime p and e > 0.
 * https://oeis.org/A393491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e || n} floor((e+1)/3) (multiplicative with a(p^e) =
 *            floor((e+1)/3) for e>0, a(1)=1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)1;

    /* Build or resize smallest‑prime‑factor table up to n */
    static idx_t *spf = NULL;          /* pointer to spf array */
    static idx_t spf_limit = 0;        /* highest index for which spf is valid */

    if (n > spf_limit) {
        if (spf) free(spf);
        build_spf(n, &spf);            /* spf[i] = smallest prime factor of i (0 for 0,1) */
        spf_limit = n;
    }

    idx_t m = n;
    seq_t prod = 1;

    /* factor m using the spf table */
    while (m > (idx_t)1) {
        idx_t p = spf[m];
        idx_t e = 0;
        while (m % p == (idx_t)0) {
            m /= p;
            ++e;
        }
        /* contribution from this prime power */
        seq_t contrib = (seq_t)((e + (idx_t)1) / (idx_t)3);
        if (contrib == (seq_t)0) return (seq_t)0; /* any zero factor makes the whole product zero */
        prod *= contrib;
    }

    return prod;
}
