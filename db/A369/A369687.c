/*
 * A369687  a(n) = Sum_{p|n, p prime} p^phi(n/p).
 * https://oeis.org/A369687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} p^phi(n/p)
 * Range:     u8 n <= 20, u16 n <= 33, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   ipow, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sum = 0;
    uint64_t tmp = n;
    uint64_t p;

    for (p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            uint64_t m = n / p;
            seq_t phi_m = totient_seq((seq_t)m);
            seq_t term   = ipow((seq_t)p, phi_m);
            sum += term;

            while (tmp % p == 0)
                tmp /= p;
        }
    }

    if (tmp > 1)
    {
        uint64_t m = n / tmp;
        seq_t phi_m = totient_seq((seq_t)m);
        seq_t term   = ipow((seq_t)tmp, phi_m);
        sum += term;
    }

    return sum;
}
