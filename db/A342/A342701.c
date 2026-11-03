/*
 * A342701  a(n) is the second smallest k such that phi(n+k) = phi(k), or 0 if
 *          no such solution exists.
 * https://oeis.org/A342701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = second smallest k with φ(n+k) = φ(k), or 0 if none (φ
 *            computed with calc_phi from the utilities)
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9934 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    /* Search for k from 1 upwards and count solutions. */
    seq_t count = 0;
    seq_t k;

    for (k = 1; ; ++k)
    {
        seq_t phi_k  = totient_seq(k);
        seq_t phi_nk = totient_seq((seq_t)(n + k));

        if (phi_k == phi_nk)
        {
            ++count;
            if (count == 2)
                return k;
        }
    }

    /* Should never reach here; return 0 for safety. */
    return 0;
}
