/*
 * A308470  a(n) = (gcd(phi(n), 4*n^2 - 1) - 1)/2, where phi is A000010,
 *          Euler's totient function.
 * https://oeis.org/A308470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (gcd(phi(n), 4*n^2-1) - 1)/2
 * Range:     u8 n <= 12, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Validated: partial, pass, 95 terms through n = 95 (2026-11-01)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* safety for sub‑offset calls */
        return 0;

    seq_t nseq  = (seq_t)n;
    seq_t phi_n = totient(n);            /* φ(n) */
    seq_t sq    = nseq * nseq;       /* n^2      */
    seq_t expr  = sq * 4 - 1;        /* 4*n^2-1  */
    seq_t g     = gcd(phi_n, expr);  /* gcd(φ(n), 4*n^2-1) */
    return (g - 1) / 2;
}
