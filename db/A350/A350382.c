/*
 * A350382  a(n) = 9 + 4*10^n.
 * https://oeis.org/A350382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9 + 4 * 10^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

/* Computes the n-th term of OEIS A350382.  Off‑by‑one offset:
   a(1) = 49, a(2) = 409, ... */
seq_t a(idx_t n)
{
    /* If n == 0 the sequence is undefined; return 13 (the value that
       coincides with 9 + 4*10^0) to avoid underflow in subsequent
       calculations.  The value will never be checked. */
    seq_t pow10n = pow10(n);
    return 4 * pow10n + 9;
}
