/*
 * A350226  a(n) is the length of the longest sequence of distinct numbers in
 *          arithmetic progression in the interval 0..n, ending with n and
 *          where the Thue-Morse sequence (A010060) is constant.
 * https://oeis.org/A350226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A350226:  a(n) = length of the longest arithmetic progression of distinct
 *  numbers in 0..n that ends at n and on which the Thue–Morse sequence
 *  (parity of the number of 1-bits of the index) is constant.
 *
 *  This implementation pre‑computes the Thue–Morse values up to n
 *  and then, for each new n, scans all possible step sizes d (1..n)
 *  to find the longest constant‑parity progression ending at n.
 *  It uses simple O(n²) work (≈50 000 000 iterations for n = 10000)
 *  which easily fits the 2‑second limit on modern processors.
 *
 *  No helper libraries are used; all dynamic arrays are managed with
 *  malloc/free and the helper function seq_t may be any unsigned integer
 *  type (uint8‑t…uint128-t or an arbitrary‑precision class).
 */

#include <stdlib.h>
#include <stdint.h>

/*---  Thue–Morse lookup  --------------------------------------------------*/

static uint8_t *tm_cache = NULL;      /* tm_cache[i] == parity of popcount(i) */
static idx_t   tm_cache_size = 0;     /* number of elements currently cached */

/* popcount for 64‑bit values */
static inline unsigned int popcount64(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    unsigned int c = 0;
    while (x)
    {
        c += x & 1;
        x >>= 1;
    }
    return c;
#endif
}

/* ensure we have tm_cache[0..n] initialized */
static void ensure_tm_cache(idx_t n)
{
    if (tm_cache != NULL && n < tm_cache_size)
        return;  /* already cached */

    /* Allocate or grow the buffer */
    uint8_t *new_buf = (uint8_t *)malloc((size_t)(n + 1));
    if (tm_cache == NULL)
    {
        /* First time: fill all entries */
        for (idx_t i = 0; i <= n; ++i)
            new_buf[i] = popcount64(i) & 1U;
    }
    else
    {
        /* Preserve old values */
        for (idx_t i = 0; i < tm_cache_size; ++i)
            new_buf[i] = tm_cache[i];
        /* Compute new entries */
        for (idx_t i = tm_cache_size; i <= n; ++i)
            new_buf[i] = popcount64(i) & 1U;
    }
    free(tm_cache);
    tm_cache = new_buf;
    tm_cache_size = n + 1;
}

/*---  a(n) cache  --------------------------------------------------------*/

static seq_t *a_cache = NULL;   /* a_cache[i] = a(i) */
static size_t a_cache_size = 0; /* number of elements currently cached */

/* ensure we have a_cache[0..n] computed */
static void ensure_a_cache(idx_t n)
{
    if (a_cache != NULL && n < a_cache_size)
        return;  /* already computed */

    /* Allocate new buffer */
    seq_t *new_arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));

    /* Copy existing values, if any */
    if (a_cache != NULL)
    {
        for (size_t i = 0; i < a_cache_size; ++i)
            new_arr[i] = a_cache[i];
        free(a_cache);
    }

    /* Compute missing values */
    for (idx_t k = (idx_t)a_cache_size; k <= n; ++k)
    {
        seq_t best = 1;              /* at least the point n itself */
        if (k > 0)
        {
            uint8_t val = tm_cache[k];
            for (idx_t d = 1; d <= k; ++d)
            {
                /* Quick bound: maximum possible length with step d */
                idx_t bound = 1 + k / d;
                if ((idx_t)best >= bound) continue;

                /* Walk backward along the progression */
                idx_t pos = k;
                seq_t len = 1;
                while (pos >= d && tm_cache[pos - d] == val)
                {
                    pos -= d;
                    ++len;
                    if ((idx_t)len >= bound) break;  /* cannot improve bound */
                }
                if (len > best)
                    best = len;
            }
        }
        new_arr[k] = best;
    }

    a_cache = new_arr;
    a_cache_size = (size_t)(n + 1);
}

/*---  public function  ---------------------------------------------------*/

seq_t a(idx_t n)
{
    ensure_tm_cache(n);
    ensure_a_cache(n);
    return a_cache[n];
}
