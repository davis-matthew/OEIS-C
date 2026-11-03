/*
 * A356170  a(n) = 1 if A001222(n) == 2*A007814(n), and otherwise 0, where
 *          A001222 is bigomega (number of prime factors with multiplicity)
 *          and A007814 is the 2-adic valuation of n.
 * https://oeis.org/A356170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if bigomega(n) == 2 * v2(n) (2‑adic valuation), else 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 138 terms through n = 8064 (2026-11-01)
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n){
    if (n < 1) return 0;
    seq_t omega = bigomega(n);          /* number of prime factors (with multiplicity) */
    idx_t tmp = n;
    idx_t v2 = 0;                       /* 2‑adic valuation */
    while ((tmp & 1) == 0) {
        v2++;
        tmp >>= 1;
    }
    seq_t two_v2 = ((seq_t)2) * (seq_t)v2;
    return (omega == two_v2) ? (seq_t)1 : (seq_t)0;
}
