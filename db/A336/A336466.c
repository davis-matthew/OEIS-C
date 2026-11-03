/*
 * A336466  Fully multiplicative with a(p) = A000265(p-1) for any prime p,
 *          where A000265(k) gives the odd part of k.
 * https://oeis.org/A336466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, look
 * Formula:   a(n) = ∏_{p^e‖n} (odd_part(p−1))^e, where odd_part(k) is
 *            A000265(k)
 * Range:     u8 n <= 522, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9537 (2026-11-01)
 * Helpers:   ipow_idx, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"
#include "../utils/odd_part.h"

/* Computes the n-th term of OEIS A336466.  The sequence is defined as
   fully multiplicative with a(p) = odd_part(p−1) for every prime p. */
seq_t a(idx_t n)
{
    /* a(1) = 1 by definition (offset 1) */
    if (n <= 1u)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* trial‑division factorisation – adequate for n up to at least 10 000 */
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            idx_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            /* a(p) = odd_part(p−1) */
            seq_t odd = odd_part((seq_t)p - (seq_t)1);
            seq_t powi = ipow_idx(odd, exp);
            result *= powi;
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1u)
    {
        seq_t odd = odd_part((seq_t)m - (seq_t)1);
        result *= odd;          /* exponent is 1 */
    }

    return result;
}
