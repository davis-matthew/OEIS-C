/*
 * A369782  a(n) = Sum_{p|n, p prime} n^phi(n/p).
 * https://oeis.org/A369782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} n^phi(n/p)
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 33, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-01)
 * Helpers:   factor_u64, ipow, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 (a(1)=0) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;                 /* a(1)=0 */

    /* Prime factorization */
    uint64_t np = (uint64_t)n;                   /* copy for arithmetic */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;
    factor_u64(np, primes, exps, &cnt);

    seq_t total = (seq_t)0;
    seq_t n_seq = (seq_t)np;                      /* use seq_t for powers */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t m = np / p;                      /* n / p  */

        seq_t phi_m = totient_seq((seq_t)m);        /* φ(n/p)  */
        seq_t term  = ipow(n_seq, phi_m);     /* n^φ(n/p) */

        total += term;
    }

    return total;
}
