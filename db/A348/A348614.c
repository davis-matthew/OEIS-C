/*
 * A348614  Numbers k such that the k-th composition in standard order has sum
 *          equal to twice its alternating sum.
 * https://oeis.org/A348614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th non‑negative integer k such that the k‑th
 *            composition in standard order has sum equal to twice its
 *            alternating sum
 * Range:     u8 n <= 25, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

 /* Cache of already generated terms.  A capacity of 500 000
    is sufficient for the expected range of n (up to about 10 000). */
static seq_t  cache[500000];
static size_t cache_len = 0;   /* number of stored values */
static idx_t  next_k   = 0;    /* smallest k not yet examined */

/* Test whether k satisfies the condition.  Uses only 64‑bit
   arithmetic, which is enough for k up to 2^64‑1. */
static inline bool satisfies(idx_t k)
{
    if (k == 0)                 /* empty composition */
        return true;            /* 0 == 2*0 */

    /* Collect positions of 1‑bits (1‑based).  pos[] holds them in
       ascending order. */
    uint32_t pos[64];
    int      m = 0;
    idx_t    t = k;
    while (t) {
        int i = __builtin_ctzll(t);        /* index of least set bit */
        pos[m++] = i + 1;                  /* 1‑based position */
        t &= t - 1;                        /* clear that bit */
    }

    /* Sum of parts is the last position. */
    idx_t sum = pos[m-1];

    /* Alternating sum of the composition.  The composition’s parts
       are the first differences of pos[], but listed in reverse
       order.  Sign pattern starts with + for the first part. */
    int64_t alt = 0;
    int prev = 0;
    for (int j = 0; j < m; ++j) {
        int part = pos[j] - prev;
        int sign = ((m - 1 - j) & 1) ? -1 : +1;  /* +,-,+,-,... */
        alt += sign * part;
        prev = pos[j];
    }

    /* Condition: sum == 2 * alternating_sum */
    return (sum == (idx_t)(2 * alt));
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined offset: return 0 */
        return 0;

    while (cache_len < (size_t)n) {                /* generate more terms if needed */
        if (satisfies(next_k))
            cache[cache_len++] = (seq_t)next_k;
        ++next_k;
    }

    return cache[n - 1];
}
