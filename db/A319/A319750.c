/*
 * A319750  a(n) is the denominator of the Heron sequence with h(0) = 3.
 * https://oeis.org/A319750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=3; Define h_n(0)=3, h_d(0)=1. hn(n+1) = (hn(n)^2 +
 *            13*hd(n)^2)/2 hd(n+1) = hn(n) * hd(n) The required a(n) is hd(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive fingerprinting of the Heron sequence denominators. */

#include <stdlib.h>

static seq_t *hd = NULL;           /* Stores hd(n) */
static seq_t *hn = NULL;           /* Stores hn(n) */
static size_t cache_cap = 0;       /* Current allocated capacity of arrays  */
static idx_t max_comp = (idx_t)-1; /* Highest index that has been computed */

static void init_cache(void)
{
    /* Allocate initial storage for indices 0 and 1. */
    cache_cap = 2;
    hd  = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    hn  = (seq_t *)malloc(cache_cap * sizeof(seq_t));

    /* Base values. */
    hd[0] = (seq_t)1;
    hn[0] = (seq_t)3;

    /* Compute index 1 from the recurrence. */
    hd[1] = hn[0] * hd[0];
    hn[1] = (hn[0] * hn[0] + (seq_t)13 * hd[0] * hd[0]) / (seq_t)2;

    max_comp = 1;
}

static void ensure_cache(idx_t n)
{
    if (max_comp == (idx_t)-1)  /* First time we are called */
        init_cache();

    if (n <= max_comp)
        return;

    /* Grow capacity if necessary. */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap << 1;   /* double */
        if (new_cap <= (size_t)n + 1)
            new_cap = (size_t)n + 1;
        hd = (seq_t *)realloc(hd, new_cap * sizeof(seq_t));
        hn = (seq_t *)realloc(hn, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing entries up to n. */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t hn_prev = hn[i - 1];
        seq_t hd_prev = hd[i - 1];

        hd[i] = hn_prev * hd_prev;  /* hd(n+1) = hn(n)*hd(n) */

        /* hn(n+1) = (hn(n)^2 + 13*hd(n)^2)/2 */
        seq_t hn_i = (hn_prev * hn_prev
                      + (seq_t)13 * hd_prev * hd_prev) / (seq_t)2;
        hn[i] = hn_i;
    }

    max_comp = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return hd[n];
}
