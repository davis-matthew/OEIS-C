/*
 * A345309  Numbers whose digital sum coincides with digital sum of their
 *          largest proper divisor.
 * https://oeis.org/A345309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that digit_sum(n) == digit_sum(largest proper
 *            divisor of n)
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   digit_sum, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the sequence on demand, storing found terms in a static cache. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/grow_seq.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                     /* out of range – arbitrary */

    /* static cache shared by all calls */
    static seq_t *terms   = NULL;                    /* stored terms */
    static size_t cap     = 0;                       /* allocated capacity */
    static idx_t  count   = 0;                       /* number of stored terms */
    static uint64_t next  = 1;                       /* last tested candidate */

    /* initialize cache on first call */
    if (!terms) grow_seq(&terms, &cap, 64);           /* start with 64 slots */

    /* generate terms until we have at least n of them */
    while (count < n) {
        ++next;                                      /* candidate number */

        /* find smallest prime factor (spf) */
        uint64_t spf = 0;
        for (uint64_t d = 2; d * d <= next; ++d) {
            if (next % d == 0) { spf = d; break; }
        }
        if (spf == 0) spf = next;                    /* prime number */

        /* largest proper divisor = next / spf, or 1 if next is prime */
        uint64_t lpd = (spf == next) ? 1ULL : next / spf;

        /* compare digit sums */
        seq_t ds_cand = digit_sum((seq_t)next);
        seq_t ds_lpd  = digit_sum((seq_t)lpd);

        if (ds_cand == ds_lpd) {
            /* ensure enough capacity */
            if (count == cap) grow_seq(&terms, &cap, count + 1);
            terms[count++] = (seq_t)next;
        }
    }

    return terms[n - 1];
}
