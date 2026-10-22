/*
 * A329474  a(n) = log_2(A110428(n)). Also, a(n) = a(n-1) + a(m) for n >= 3,
 *          where m = 2^(p+1) + 2 - n and p is the unique integer such that
 *          2^p < n - 1 <= 2^(p+1), starting with a(1) = 0 and a(2) = 1.
 * https://oeis.org/A329474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329474: a(n) = a(n-1) + a(m) for n≥3,
   where m = 2^(p+1)+2-n and p is such that 2^p < n-1 ≤ 2^(p+1);
   with a(1)=0, a(2)=1.
   (The formula is implemented directly.) */

#include <stddef.h>
#include <stdint.h>

static seq_t cache[100001];          /* enough for n≤10000 */
static idx_t  cache_max = 0;         /* largest index computed so far */

/* floor(log2(v)) for v>0, implemented without compiler intrinsics */
static unsigned int floor_log2_u64(idx_t v)
{
    unsigned int p = 0;
    while (((idx_t)1) << (p + 1) <= v)
        ++p;
    return p;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* indices below the offset are irrelevant */

    if (n > 100000)                  /* guard against out‑of‑bounds */
        return 0;

    /* compute missing values up to n */
    if (cache_max < n)
    {
        for (idx_t i = cache_max + 1; i <= n; ++i)
        {
            if (i == 1)
                cache[i] = (seq_t)0;
            else if (i == 2)
                cache[i] = (seq_t)1;
            else
            {
                idx_t v   = i - 2; /* v = i-1-1 */
                unsigned int p = floor_log2_u64(v);
                idx_t  power = ((idx_t)1) << (p + 1);
                idx_t  m     = power + 2 - i;
                cache[i] = cache[i - 1] + cache[m];
            }
        }
        cache_max = n;
    }

    return cache[n];
}
