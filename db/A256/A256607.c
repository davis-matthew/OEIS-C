/*
 * A256607  Eventual period of 2^(2^k) mod n.
 * https://oeis.org/A256607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007733(A007733(n))
 * Range:     u8 n <= 585, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sq_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the eventual period of the squaring map x → x² mod n
   starting from x = 2.  The value returned is the length of the
   cycle that is eventually reached. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sq_mod.h"

seq_t a(idx_t n) {
    /* The sequence is defined for n ≥ 1; for n = 0 we simply
       return 0, but such calls will not occur in tests. */
    if (n == 0) return 0;

    /* Modulus as seq_t ensures everything is computed in the
       correctly sized integer type. */
    seq_t mod = (seq_t)n;

    /* Starting value is 2 modulo n.  For n = 1, this is 0. */
    seq_t start = (seq_t)2 % mod;

    /* Floyd’s cycle‑finding algorithm for the map f(x) = x² mod n. */
    seq_t tortoise = sq_mod(start, mod);          /* f(start)          */
    seq_t hare = sq_mod(sq_mod(start, mod), mod); /* f(f(start))       */

    /* Find first meeting point inside the cycle. */
    while (tortoise != hare) {
        tortoise = sq_mod(tortoise, mod);
        hare = sq_mod(sq_mod(hare, mod), mod);
    }

    /* Find the start of the cycle (μ).  Not really needed for the
       cycle length, but shown for completeness. */
    seq_t mu_start = start;
    seq_t mu = 0;
    while (mu_start != hare) {
        mu_start = sq_mod(mu_start, mod);
        hare = sq_mod(hare, mod);
        ++mu;
    }

    /* Find the cycle length λ. */
    seq_t lambda = 1;
    hare = sq_mod(mu_start, mod);
    while (mu_start != hare) {
        hare = sq_mod(hare, mod);
        ++lambda;
    }

    return lambda;
}
