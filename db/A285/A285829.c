/*
 * A285829  Numbers n such that, for any i and j with i >= j >= 0, ds^i(n)
 *          divides ds^j(n) (where ds^k denotes the k-th iteration of the
 *          digital sum).
 * https://oeis.org/A285829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 5939, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search implementation: a(n) is the n-th integer such that for all i ≥ j ≥ 0,
   ds^i(n) divides ds^j(n), where ds denotes the digital sum. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_sum.h"

/* A statically‑allocated cache; 200 000 is far more than enough to hold
   the first several tens of thousands of terms.  The array size can be
   increased if needed. */
static seq_t cache[200000];
static size_t cache_len = 0;
static seq_t next_candidate = 1;

/* Test whether a single integer satisfies the chain–divisibility
   property. */
static bool good(seq_t n)
{
    seq_t cur = n;
    seq_t ds = digit_sum(cur);
    if (cur % ds != 0)
        return false;
    cur = ds;
    while (cur >= (seq_t)10)
    {
        seq_t nxt = digit_sum(cur);
        if (cur % nxt != 0)
            return false;
        cur = nxt;
    }
    return true;
}

/* Return the n-th term (1‑based offset).  The function builds and
   preserves a cache, so subsequent calls are fast even when n is not
   monotone. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* no term for offset 0 */
    while (cache_len < (size_t)n)
    {
        if (good(next_candidate))
            cache[cache_len++] = next_candidate;
        next_candidate++;
    }
    return cache[n-1];
}
