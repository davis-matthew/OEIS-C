/*
 * A250007  Run lengths in A100618.
 * https://oeis.org/A250007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of equal consecutive terms in
 *            A100618 (computed by scanning A100618(m), m = 1, 2, ...)
 * Range:     u8 n <= 237, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 8720 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A100618_a(idx_t n)
{
    seq_t M = (seq_t)n;
    for (idx_t k = 2; ; ++k)
    {
        seq_t ksqr = (seq_t)k * (seq_t)k;
        if (ksqr > M)
            break;
        seq_t t = M / ksqr;
        M -= t;
    }
    return M;
}

seq_t a(idx_t n)
{
    seq_t prev = A100618_a(1);
    seq_t len = 1;
    idx_t run = 1;
    for (idx_t m = 2; ; ++m)
    {
        seq_t v = A100618_a(m);
        if (v == prev)
        {
            ++len;
        }
        else
        {
            if (run == n)
                return len;
            ++run;
            prev = v;
            len = 1;
        }
    }
}
