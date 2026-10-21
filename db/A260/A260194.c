/*
 * A260194  a(n+1) = a(n) + gcd(a(n),a(n-2)), with a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A260194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=a(3)=1, a(n+1)=a(n)+gcd(a(n),a(n-2)) (n≥3)
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Cached sequence values. Index 0 corresponds to a(1). */
    static seq_t *seq = NULL;
    static size_t len = 0;   /* number of terms currently stored */
    static size_t cap = 0;   /* allocated capacity */

    /* Convert index to size_t for array bounds, okay for typical n. */
    size_t target = (size_t)n;
    if (target == 0) /* n < 1 is undefined; return 0 as safety */ return 0;

    /* Initialize cache if first call. */
    if (len == 0) {
        cap = 8;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return 0;          /* allocation failure fallback */
        seq[0] = 1; /* a(1) */
        seq[1] = 1; /* a(2) */
        seq[2] = 1; /* a(3) */
        len   = 3;
    }

    /* Extend the cache up to the requested index. */
    while (len < target) {
        if (len >= cap) { /* grow capacity */
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) return 0;      /* allocation failure fallback */
            seq = tmp;
            cap = new_cap;
        }
        /* Recurrence: a(n+1) = a(n) + gcd(a(n), a(n-2)) */
        seq_t cur = seq[len - 1];           /* a(n) where n = len */
        seq_t prev2 = seq[len - 3];         /* a(n-2) */
        seq_t g = gcd(cur, prev2);
        seq[len] = cur + g;
        ++len;
    }

    /* Return the requested term (1‑based index). */
    return seq[target - 1];
}
