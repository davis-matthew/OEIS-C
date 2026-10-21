/*
 * A285899  Total number of parts in all partitions of all positive integers
 *          <= n into consecutive parts.
 * https://oeis.org/A285899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A285899 – total number of parts in all partitions of all integers
   ≤ n into consecutive parts.  The function computes
   a(n) = Σ_{k=1..n} f(k),
   where f(k) is the sum, over all partitions of k into consecutive
   positive integers, of the number of parts in each partition.
   For n ≥ 1, a(1)=1, a(2)=2, a(3)=5, … as in the OEIS entry.   */
#include <stdlib.h>
#include <stdint.h>

/*----- utility: integer square root (floor) for unsigned 64‑bit values -----*/
static inline idx_t isqrt(idx_t n)
{
    idx_t low  = 0;
    /* The maximum possible sqrt fits in 32 bits for a 64‑bit input. */
    idx_t high = 0xFFFFFFFFULL;
    if (high > n) high = n;

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        /* Avoid over‑overflow: mid*mid > n  iff  mid > n/mid */
        if (mid > 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/*----- memoised prefix sums:  a(0)=0 (unused) -----*/
static seq_t *prefix = NULL;
static idx_t  pref_size = 0;

/*----- main function for the OEIS sequence -----*/
seq_t a(idx_t n)
{
    if (n == 0)               /* the sequence is defined for n≥1 */
        return 0;

    /* Initialise the memoization table on the first call */
    if (pref_size == 0) {
        pref_size = 1;
        /* cast is required for C++ compatibility */
        prefix = (seq_t *)malloc(sizeof(seq_t));
        if (!prefix) return 0;          /* allocation failure – unlikely */
        prefix[0] = 0;                  /* a(0) = 0 */
    }

    /* Extend the cache up to the requested n */
    if (pref_size <= n) {
        /* Allocate space for indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(prefix, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – unlikely */
        prefix = tmp;

        /* Compute values for i = pref_size … n */
        for (idx_t i = pref_size; i <= n; ++i) {
            seq_t parts_sum = 0;       /* f(i) */

            /* Maximum length m satisfies m(m+1)/2 ≤ i.
               Compute m_max = floor((sqrt(8*i+1)-1)/2).           */
            idx_t m_max = (isqrt((idx_t)(8 * i + 1)) - 1) / 2;

            for (idx_t m = 1; m <= m_max; ++m) {
                /* For a consecutive‑part partition of i of length m,
                   2*i must be divisible by m (see the derivation
                   of the arithmetic‑progression sum).             */
                if ((2 * i) % m != 0) continue;

                idx_t t = (2 * i) / m;            /* 2*a_start + m – 1 */
                /* a_start integer requires that t and m have opposite parity. */
                if ((t & 1) == (m & 1)) continue;

                idx_t a_start = (t - m + 1) / 2;
                if (a_start >= 1)                 /* positive parts only */
                    parts_sum += (seq_t)m;         /* add the number of parts */
            }

            prefix[i] = prefix[i - 1] + parts_sum;  /* a(i) */
        }

        pref_size = n + 1;
    }

    return prefix[n];
}
