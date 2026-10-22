/*
 * A378191  a(n) is the number of integer bases >= 2 in which n is digitally
 *          balanced.
 * https://oeis.org/A378191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the number of integer bases b ≥ 2 in which n is digitally
   balanced, i.e. in the base‑b representation every digit 0…b‑1 occurs the
   same number of times.  This implementation simply tries all bases up
   to n (after which the representation has fewer digits than the base)
   and counts those that satisfy the property. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Trivial cases */
    if (n <= 1U)
        return 0;

    /* Count of balanced bases */
    seq_t count = 0;

    /* Static digit counter reused for each base; reallocated only when
       a larger base is needed. */
    static unsigned *cnt = NULL;  /* digit counters 0 … b‑1 */
    static idx_t cap = 0;         /* current maximum capacity */

    for (idx_t b = 2; ; ++b)
    {
        /* Grow the counter array if necessary */
        if (b > cap)
        {
            cnt = (unsigned *)realloc(cnt,
                                       (size_t)b * sizeof(unsigned));
            if (!cnt) abort();    /* allocation failure */
            cap = b;
        }

        /* Zero counters for digits 0 … b‑1 */
        for (idx_t i = 0; i < b; ++i)
            cnt[i] = 0;

        /* Convert n to base‑b and count digits */
        idx_t tmp = n;
        idx_t len = 0;           /* number of digits in this base */
        while (tmp > 0)
        {
            idx_t d = tmp % b;
            cnt[d] += 1;
            tmp    /= b;
            ++len;
        }

        /* If the representation has fewer digits than the base,
           no larger base can be balanced – terminate the loop. */
        if (len < b)
            break;

        /* For a balanced representation each digit must occur exactly
           len / b times, so len must be divisible by b. */
        if (len % b != 0)
            continue;

        idx_t needed = len / b;      /* required count for every digit */
        bool ok = true;
        for (idx_t i = 0; i < b; ++i)
        {
            if (cnt[i] != needed)
            {
                ok = false;
                break;
            }
        }

        if (ok)
            ++count;
    }

    return count;
}
