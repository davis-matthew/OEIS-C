/*
 * A381067  Expansion of e.g.f. log(1-x)^2 * exp(-x) / (2 * (1-x)).
 * https://oeis.org/A381067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) *
 *            |Stirling1(k+1,3)|
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* ensure_capacity: enlarges *ptr so that it can hold at least 'needed' elements of size 'elem_size' */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap < needed)
    {
        size_t new_cap = (*cap == 0 ? 1 : *cap << 1);
        while (new_cap < needed)
            new_cap <<= 1;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

/* static storage for factorials, unsigned Stirling numbers of the first kind (k=2,3) and the sequence a(n) */
static seq_t *fac_arr   = NULL;
static size_t fac_cap   = 0;
static idx_t  fac_max   = 1;    /* highest index computed in fac_arr   */

static seq_t *c2_arr   = NULL;
static size_t c2_cap   = 0;
static idx_t  c2_max   = 1;    /* highest index computed in c2_arr   */

static seq_t *c3_arr   = NULL;
static size_t c3_cap   = 0;
static idx_t  c3_max   = 2;    /* highest index computed in c3_arr   */

static seq_t *a_cache  = NULL;
static size_t a_cap    = 0;
static idx_t  a_max    = 0;    /* highest index computed in a_cache */

/* precompute factorials, c2, c3 up to n+1 */
static void precompute(idx_t n)
{
    /* ensure factorial array has enough capacity */
    if (fac_arr == NULL)
    {
        ensure_capacity(2, &fac_cap, (void **)&fac_arr, sizeof(seq_t));
        fac_arr[0] = (seq_t)1;
        fac_arr[1] = (seq_t)1;
    }

    if (c2_arr == NULL)
    {
        ensure_capacity(2, &c2_cap, (void **)&c2_arr, sizeof(seq_t));
        c2_arr[0] = (seq_t)0;
        c2_arr[1] = (seq_t)0;
    }

    if (c3_arr == NULL)
    {
        ensure_capacity(3, &c3_cap, (void **)&c3_arr, sizeof(seq_t));
        c3_arr[0] = (seq_t)0;
        c3_arr[1] = (seq_t)0;
        c3_arr[2] = (seq_t)0;
    }

    /* extend arrays up to n+1 */
    if (n + 1 > fac_max)
    {
        size_t needed = (size_t)(n + 2);  /* need indices 0 .. n+1 inclusive */
        ensure_capacity(needed, &fac_cap, (void **)&fac_arr, sizeof(seq_t));
        ensure_capacity(needed, &c2_cap,  (void **)&c2_arr,  sizeof(seq_t));
        ensure_capacity(needed, &c3_cap,  (void **)&c3_arr,  sizeof(seq_t));

        for (idx_t i = fac_max + 1; i <= n + 1; ++i)
        {
            if (i == 0)         fac_arr[0] = (seq_t)1;
            else if (i == 1)    fac_arr[1] = (seq_t)1;
            else                fac_arr[i] = fac_arr[i - 1] * (seq_t)i;

            if (i < 2)          c2_arr[i] = (seq_t)0;
            else                c2_arr[i] = fac_arr[i - 2] + ((seq_t)(i - 1)) * c2_arr[i - 1];

            if (i < 2)          c3_arr[i] = (seq_t)0;
            else if (i == 2)    c3_arr[2] = (seq_t)0;
            else                c3_arr[i] = c2_arr[i - 1] + ((seq_t)(i - 1)) * c3_arr[i - 1];
        }
        fac_max = n + 1;
        c2_max  = n + 1;
        c3_max  = n + 1;
    }

    /* allocate cache for a(n) if needed */
    if (a_cache == NULL)
    {
        ensure_capacity(1, &a_cap, (void **)&a_cache, sizeof(seq_t));
        a_cache[0] = (seq_t)0;     /* a(0) = 0 */
        a_max = 0;
    }

    /* compute missing a(i) values up to n */
    if (n > a_max)
    {
        ensure_capacity((size_t)(n + 1), &a_cap, (void **)&a_cache, sizeof(seq_t));
        for (idx_t i = a_max + 1; i <= n; ++i)
        {
            seq_t pos = (seq_t)0;
            seq_t neg = (seq_t)0;
            seq_t binom = (seq_t)1;  /* binomial(i,0) */

            for (idx_t k = 0; k <= i; ++k)
            {
                if (k > 0)
                {
                    /* binom = binom * (i - k + 1) / k */
                    binom = (binom * (seq_t)(i - k + 1)) / (seq_t)k;
                }

                /* use |Stirling1(k+1,3)| = c3_arr[k+1] */
                seq_t c = (k + 1 <= c3_max) ? c3_arr[k + 1] : (seq_t)0;

                if (c != (seq_t)0)
                {
                    seq_t prod = binom * c;
                    if (((i - k) & 1ULL) == 0)
                        pos += prod;
                    else
                        neg += prod;
                }
            }

            /* pos >= neg for the whole sum */
            a_cache[i] = pos - neg;
        }
        a_max = n;
    }
}

/* exported function */
seq_t a(idx_t n)
{
    precompute(n);
    return a_cache[n];
}
