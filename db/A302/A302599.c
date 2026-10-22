/*
 * A302599  Numbers k such that digit_sum(k) > digit_sum(2k).
 * https://oeis.org/A302599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth integer k such that digit_sum(k) > digit_sum(2k)
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the sequence up to the requested index and caches
   results for future calls.  The sequence is obtained by brute‑force
   enumeration of positive integers and checking the digit‑sum inequality. */

#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Static cache of produced terms */
    static seq_t *cache = NULL;    /* array of a(i) values */
    static size_t cap = 0;         /* allocated capacity of cache */
    static size_t len = 0;         /* number of terms currently stored */
    static idx_t last_k = 0;       /* last integer k examined */

    if (n == 0)  /* undefined; return 0 as a benign fallback */
        return 0;

    /* Generate terms until we have at least n of them */
    while (len < n)
    {
        idx_t k = last_k + 1;   /* next integer to test */
        last_k = k;

        seq_t k_val = (seq_t)k;              /* cast to seq_t for digit_sum */
        seq_t s1     = digit_sum(k_val);     /* digit sum of k */
        seq_t s2     = digit_sum(k_val + k_val); /* digit sum of 2k */

        if (s1 > s2)   /* k satisfies condition, record it */
        {
            if (len == cap)                       /* expand storage if needed */
                grow_seq(&cache, &cap, len + 1);

            cache[len++] = k_val;
        }
    }

    return cache[n - 1];   /* zero‑based index into cached array */
}
