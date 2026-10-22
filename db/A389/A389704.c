/*
 * A389704  a(n) = maximum k such that B(n) > B(n+1) > ... > B(n+k-1), where B
 *          = bigomega (A001222).
 * https://oeis.org/A389704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum k such that bigomega(n) > bigomega(n+1) > ... >
 *            bigomega(n+k-1)
 * Range:     u8 n <= 9749, u16 n <= 9749, u32 n <= 9749, u64 n <= 9749,
 *            u128 n <= 9749, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    seq_t cur = bigomega(n);
    seq_t count = 1;
    idx_t curIdx = n;

    while (1)
    {
        idx_t nextIdx = curIdx + 1;
        seq_t next = bigomega(nextIdx);
        if (next < cur)
        {
            ++count;
            cur = next;
            curIdx = nextIdx;
        }
        else
        {
            break;
        }
    }
    return count;
}
