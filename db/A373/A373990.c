/*
 * A373990  a(n) = 1 if the binary weight of n is not greater than the 2-adic
 *          valuation of 2*n, otherwise 0.
 * https://oeis.org/A373990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the binary weight of n is not greater than the
 *            2‑adic valuation of 2*n, otherwise 0. That is, a(n) =
 *            [A000120(n) <= A001511(n)] where A000120(n) is popcount(n) and
 *            A001511(n) = v2(2*n) = v2(n)+1
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 136 terms through n = 8849 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is assumed >= 1 (offset of the sequence) */
    unsigned long long x = (unsigned long long)n;

    /* binary weight of n */
    unsigned pop = __builtin_popcountll(x);

    /* number of trailing zeros in n (v2(n)) */
    unsigned tz = __builtin_ctzll(x);

    /* v2(2*n) = v2(n) + 1 */
    unsigned val2 = tz + 1;

    return (seq_t)(pop <= val2 ? 1 : 0);
}
