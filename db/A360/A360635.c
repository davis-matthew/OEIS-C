/*
 * A360635  a(n) is the smallest nonnegative integer that satisfies p(a(n)+1)
 *          - p(a(n)) >= n, where p denotes the number partition function.
 * https://oeis.org/A360635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k ≥ 0 | p(k+1) – p(k) ≥ n} where p is the partition
 *            function. The routine computes p(k) via the recurrence using the
 *            generalized pentagonal numbers and builds the differences d[k] =
 *            p(k+1) – p(k). It lazily expands the cached tables as needed
 * Range:     u8 n <= 66, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for partitions and differences */
static seq_t *p       = NULL;   /* p[0..]  partition numbers */
static seq_t *d       = NULL;   /* d[0..]  differences p[k+1]-p[k] */

/* partition array parameters */
static idx_t  p_cap   = 0;      /* capacity of p array */
static idx_t  p_done  = 0;      /* largest index computed in p */

/* difference array parameters */
static idx_t  d_cap   = 0;      /* capacity of d array */
static idx_t  d_done  = 0;      /* largest index computed in d */

/*  Ensure p array has capacity up to index 'up_to' (inclusive)
    and that all p[0..up_to] are computed. */
static void ensure_partitions(idx_t up_to)
{
    if (up_to <= p_done)
        return;

    /* grow capacity if necessary */
    if (up_to + 1 > p_cap) {
        size_t new_cap = (p_cap == 0) ? (size_t)(up_to + 1) : (size_t)(p_cap * 2);
        if (new_cap < (size_t)(up_to + 1))
            new_cap = up_to + 1;
        seq_t *tmp = (seq_t *)realloc(p, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                       /* unrecoverable */
        for (idx_t i = p_cap; i < (idx_t) new_cap; ++i)
            tmp[i] = 0;
        if (p_cap == 0)
            tmp[0] = 1;                    /* p(0) = 1 */
        p   = tmp;
        p_cap = new_cap;
    }

    /* compute missing partition numbers using the generalized pentagonal
       number theorem.  For n >= 1:  p(n) = Σ (-1)^{k+1} p(n - G(k)),
       where G(k) = k(3k±1)/2.  We split the sum into positive and negative
       parts to avoid unsigned subtraction. */
    for (idx_t n = p_done + 1; n <= up_to; ++n) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = (idx_t)(k * (3 * k - 1) / 2);
            if (g1 > n)
                break;
            if (k & 1)
                pos += p[n - g1];
            else
                neg += p[n - g1];

            idx_t g2 = (idx_t)(k * (3 * k + 1) / 2);
            if (g2 > n)
                continue;
            if (k & 1)
                pos += p[n - g2];
            else
                neg += p[n - g2];
        }
        p[n] = pos;
        if (neg != 0)
            p[n] -= neg;            /* pos ≥ neg is guaranteed */
    }
    p_done = up_to;
}

/*  Ensure difference array has capacity up to index 'up_to' (inclusive)
    and that all differences d[0..up_to] are computed. */
static void ensure_diffs(idx_t up_to)
{
    if (up_to <= d_done)
        return;

    /* we need p up to up_to+1 to compute differences */
    ensure_partitions(up_to + 1);

    /* grow capacity if necessary */
    if (up_to + 1 > d_cap) {
        size_t new_cap = (d_cap == 0) ? (size_t)(up_to + 1) : (size_t)(d_cap * 2);
        if (new_cap < (size_t)(up_to + 1))
            new_cap = up_to + 1;
        seq_t *tmp = (seq_t *)realloc(d, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        /* newly allocated slots are already zeroed by realloc */
        d   = tmp;
        d_cap = new_cap;
    }

    /* compute missing differences */
    for (idx_t k = d_done + 1; k <= up_to; ++k)
        d[k] = p[k + 1] - p[k];

    d_done = up_to;
}

/*  Sequence function a(n) as per OEIS A360635.  */
seq_t a(idx_t n)
{
    /* special case: a(0) = 0 */
    if (n == 0)
        return 0;

    /* make sure d[0] exists */
    ensure_diffs(0);

    /* find an upper bound where d[high] >= n */
    idx_t high = d_done;          /* last computed difference index */
    if (high == 0)                /* ensure at least index 1 exists */
        high = 1;

    ensure_diffs(high);

    while (d[high] < n) {
        idx_t new_high = (high == 0) ? 1 : high * 2;
        ensure_diffs(new_high);
        high = new_high;
    }

    /* binary search for minimal k with d[k] >= n */
    idx_t low = 0;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (d[mid] >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}
