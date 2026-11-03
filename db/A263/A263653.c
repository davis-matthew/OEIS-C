/*
 * A263653  a(n) = bigomega(n)^omega(n).
 * https://oeis.org/A263653
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(n)^omega(n)
 * Range:     u8 n <= 209, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 135 terms through n = 4849 (2026-11-01)
 * Helpers:   bigomega, ipow_idx, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/ipow_idx.h"
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    /* For n < 2 the OEIS offset is not defined; return 1 as a harmless default. */
    if (n < 2) return (seq_t)1;

    /* total number of prime factors of n (with multiplicity) */
    seq_t w = bigomega(n);

    /* number of distinct prime factors of n */
    int distinct = omega((uint64_t)n);

    /* if n is prime, omega(n) == 1, so a(n) = bigomega(n) == 1 */
    if (distinct <= 1) return w;

    return ipow_idx(w, (idx_t)distinct);
}
