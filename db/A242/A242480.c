/*
 * A242480  a(n) = (n*(n+1)/2) mod n + sigma(n) mod n + antisigma(n) mod n.
 * https://oeis.org/A242480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(n+1)/2) mod n + sigma(n) mod n + antisigma(n) mod n
 *            with antisigma(n) = (n*(n+1)/2) - sigma(n)
 * Range:     u8 n <= 22, u16 n <= 1253, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of_u64.h"

/* Main sequence function required by the build. */
seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    seq_t tn = (seq_t)n;
    seq_t T  = tn * (tn + (seq_t)1) / (seq_t)2;   /* n*(n+1)/2 */

    seq_t mod_tri   = T % tn;                    /* (n*(n+1)/2) mod n */
    seq_t sigma     = sigma_of_u64((uint64_t)n);
    seq_t mod_sigma = sigma % tn;                /* sigma(n) mod n */

    seq_t antisigma = T - sigma;                 /* antisigma(n) = T - sigma(n) */
    seq_t mod_antis = antisigma % tn;            /* antisigma(n) mod n */

    return mod_tri + mod_sigma + mod_antis;
}
