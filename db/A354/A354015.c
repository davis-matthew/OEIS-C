/*
 * A354015  Expansion of e.g.f. 1/(1 - x)^(1 - log(1-x)).
 * https://oeis.org/A354015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} A000776(k-1) * C(n-1,k-1) *
 *            a(n-k), where A000776(k-1) = (1 + 2 * (1 + 1/2 + … + 1/(k-1))) *
 *            (k-2)! + (k-1) * A000776(k-2) (computed as g_k = (k-1)*g_{k-1} +
 *            2*(k-2)!) and C(m,r) is the ordinary binomial coefficient
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*======================================================================
  Internal static data for memoisation
 ======================================================================*/
#include <stdlib.h>      /* for malloc/free */

static seq_t *a_cache = NULL;    /* a values */
static seq_t *fac_cache = NULL;  /* factorials */
static seq_t *g_cache  = NULL;   /* A000776 values (g_k) */
static int    cache_max = -1;    /* largest n computed so far */

/*======================================================================
  Helper: ensure the arrays are large enough for n
 ======================================================================*/
static void ensure_capacity(idx_t n)
{
    if (cache_max >= 0 && (idx_t)cache_max >= n)
        return;

    size_t new_size = (size_t)(n + 1);

    /* Allocate new arrays */
    seq_t *new_a  = (seq_t*)malloc(new_size * sizeof(seq_t));
    seq_t *new_f  = (seq_t*)malloc(new_size * sizeof(seq_t));
    seq_t *new_g  = (seq_t*)malloc(new_size * sizeof(seq_t));

    /* Copy old data */
    if (a_cache)
    {
        for (idx_t i = 0; i <= (idx_t)cache_max; ++i)
        {
            new_a[i] = a_cache[i];
            new_f[i] = fac_cache[i];
            new_g[i] = g_cache[i];
        }
    }

    /* Free old memory */
    if (a_cache)  free(a_cache);
    if (fac_cache) free(fac_cache);
    if (g_cache)  free(g_cache);

    a_cache = new_a;
    fac_cache = new_f;
    g_cache = new_g;

    /* Initialise new entries up to current max */
    for (idx_t i = (cache_max >= 0 ? (idx_t)cache_max + 1 : 0);
         i <= n; ++i)
    {
        /* factorials */
        if (i == 0)
            fac_cache[0] = (seq_t)1;
        else
            fac_cache[i] = fac_cache[i-1] * (seq_t)i;

        /* g values (A000776) */
        if (i == 0)
            ;          /* unused */
        else if (i == 1)
            g_cache[1] = (seq_t)1;
        else
            /* g[i] = (i-1) * g[i-1] + 2 * fac[i-2] */
            g_cache[i] = ((seq_t)(i-1)) * g_cache[i-1] + (seq_t)2 * fac_cache[i-2];
    }

    /* Ensure a[0] exists */
    if (cache_max < 0)
    {
        a_cache[0] = (seq_t)1;
        cache_max = 0;
    }
}

/*======================================================================
  Main sequence function
  ======================================================================*/
seq_t a(idx_t n)
{
    /* Build all needed values */
    ensure_capacity(n);

    /* Compute a(n) for all missing n up to the desired one */
    for (idx_t i = (cache_max >= 0 ? (idx_t)cache_max + 1 : 1);
         i <= n; ++i)
    {
        seq_t sum   = (seq_t)0;
        seq_t comb  = (seq_t)1;             /* C(i-1,0) */
        for (idx_t k = 1; k <= i; ++k)
        {
            /* term = g[k] * comb * a[i-k] */
            sum += g_cache[k] * comb * a_cache[i-k];

            /* Update combination for next k:
               C(i-1,k) = C(i-1,k-1) * (i-k) / k
            */
            if (k < i)
                comb = (comb * (seq_t)(i - k)) / (seq_t)k;
        }
        a_cache[i] = sum;
    }

    cache_max = (int)n;
    return a_cache[n];
}
