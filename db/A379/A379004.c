/*
 * A379004  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => v_2(i) = v_2(j) and v_5(i) = v_5(j), for all i, j, where v_2
 *          (A007814) and v_5 (A112765) give the 2- and 5-adic valuations of n
 *          respectively.
 * https://oeis.org/A379004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 9654, u16 n <= 9654, u32 n <= 9654, u64 n <= 9654,
 *            u128 n <= 9654, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A379004: lexicographically earliest sequence with distinct
   (v2, v5) pairs.  For each n (n ≥ 1) let v2 = exponent of 2 in n,
   v5 = exponent of 5 in n.  Whenever a new pair (v2, v5) is seen,
   assign it the smallest unused integer, creating a restricted growth
   sequence.  The first term is 1, the second is 2, etc.  */

#include <stdint.h>

static seq_t pair_label[65][65] = {0};
static seq_t next_label = 1;

seq_t a(idx_t n)
{
    /* compute v2(n) */
    unsigned int v2 = 0;
    uint64_t t = n;
    while ((t & 1ULL) == 0) {
        ++v2;
        t >>= 1;
    }

    /* compute v5(n) */
    unsigned int v5 = 0;
    while ((t % 5ULL) == 0) {
        ++v5;
        t /= 5ULL;
    }

    seq_t val = pair_label[v2][v5];
    if (val == 0) {
        val = next_label++;
        pair_label[v2][v5] = val;
    }
    return val;
}
