/*
 * A260894  G.f.: Sum_{n>=0} x^n * Product_{k=1..n} (1 - x^(n+k))/(1 - x^k).
 * https://oeis.org/A260894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m>=0} (-1)^m * p(n - 3*m*(m+1)/2)
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p(k) is the partition function (p(0)=1, p(k)=0 for k<0) */

#include <stdlib.h>

/* static storage for partition numbers p(0) … p(max) */
static seq_t *partitions = NULL;   /* array of partition values */
static size_t part_size   = 0;      /* number of elements currently allocated */

/* ------------------------------------------------------------------ */
/*  Compute partition values up to and including n, if not already done */
/* ------------------------------------------------------------------ */
static void ensure_partitions(idx_t n)
{
    /* already have enough values? */
    if (part_size > (size_t)n)
        return;

    /* allocate or grow the array: indices 0 … n inclusive */
    size_t newsize = (size_t)n + 1;
    if (partitions == NULL)
        partitions = (seq_t *)malloc(newsize * sizeof(seq_t));
    else
        partitions = (seq_t *)realloc(partitions, newsize * sizeof(seq_t));

    if (partitions == NULL)   /* allocation failed – let the caller crash */
        return;

    /* compute missing partition numbers */
    for (idx_t i = part_size; i <= n; ++i) {
        if (i == 0) {           /* p(0) = 1 */
            partitions[i] = 1;
            continue;
        }
        seq_t pos = 0, neg = 0; /* separate positive and negative terms */

        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i)           /* no more pentagonal numbers */
                break;
            if ((k & 1ULL) == 0)   /* even k → negative sign */
                neg += partitions[(size_t)(i - g1)];
            else                  /* odd k → positive sign */
                pos += partitions[(size_t)(i - g1)];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > i)           /* g2 may exceed i for the second set */
                continue;
            if ((k & 1ULL) == 0)
                neg += partitions[(size_t)(i - g2)];
            else
                pos += partitions[(size_t)(i - g2)];
        }
        partitions[i] = pos - neg;   /* guaranteed non‑negative */
    }

    part_size = newsize;               /* update the current size */
}

/* ------------------------------------------------------------------ */
/*  Sequence a(n) as defined by the generating function.              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_partitions(n);              /* make sure we have p(0)…p(n) ready */

    seq_t pos = 0, neg = 0;            /* sums for the alternating series   */
    for (idx_t m = 0; ; ++m) {
        idx_t T = m * (3 * m + 3) / 2; /* equals 3*m*(m+1)/2 */
        if (T > n)                     /* terms with negative index are 0   */
            break;
        idx_t idx = n - T;
        if ((m & 1ULL) == 0)           /* (-1)^m = +1 for even m            */
            pos += partitions[(size_t)idx];
        else                           /* (-1)^m = -1 for odd m             */
            neg += partitions[(size_t)idx];
    }
    return pos - neg;                  /* result is non‑negative             */
}
