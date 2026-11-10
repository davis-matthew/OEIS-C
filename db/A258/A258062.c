/*
 * A258062  Run lengths of zeros and ones in A188967.
 * https://oeis.org/A258062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of equal consecutive terms in
 *            A188967 (A188967(1)=0, A188967(3k-2)=A188967(k),
 *            A188967(A007494(k))=1-A188967(k)); computed by scanning A188967
 *            and counting runs
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9604 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A188967_f(idx_t n)
{
    int val = 0;
    while (n > 1)
    {
        if (n % 3 == 1)
        {
            n = (n + 2) / 3;
        }
        else
        {
            val ^= 1;
            idx_t fl = (n + 2) / 3;
            n = n - fl;
        }
    }
    return val;
}

seq_t a(idx_t n)
{
    idx_t runs = 0;
    idx_t k = 1;
    int prev = A188967_f(1);
    idx_t len = 1;
    for (;;)
    {
        k++;
        int v = A188967_f(k);
        if (v == prev)
        {
            len++;
        }
        else
        {
            runs++;
            if (runs == n)
                return (seq_t)len;
            len = 1;
            prev = v;
        }
    }
}
