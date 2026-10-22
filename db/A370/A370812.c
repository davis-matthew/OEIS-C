/*
 * A370812  a(1) = 1; for n >= 2, a(n) = noz(a(n-1) + 2*n - 1), where noz(n) =
 *          A004719(n).
 * https://oeis.org/A370812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1; for n≥2, a(n)=noz(a(n-1)+2*n-1), where noz(n) removes
 *            all decimal zeros from n (OEIS A004719)
 * Range:     u8 n <= 20, u16 n <= 1709, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_digits.h"

/* removes all decimal zero digits from n */
static seq_t noz(seq_t n)
{
    /* 256 digits are enough for any seq_t value */
    uint8_t buf[256];
    size_t len = 0;
    get_digits(n, buf, &len);

    seq_t res = 0;
    /* get_digits returns digits least‑significant first; build the number
       in the proper order by iterating backwards. */
    for (int i = (int)len - 1; i >= 0; --i)
    {
        uint8_t d = buf[i];
        if (d == 0) continue;          /* skip zeros */
        res = res * 10 + d;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n<1 */

    static seq_t *cache = NULL;
    static idx_t cached_len = 0;        /* number of terms stored */

    /* initialise cache with a(1)=1 */
    if (!cache)
    {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;           /* allocation failed */
        cache[0] = 1;
        cached_len = 1;
    }

    /* grow cache up to requested n (if necessary) */
    if (n > cached_len)
    {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failed */
        cache = tmp;

        for (idx_t i = cached_len; i < new_len; ++i)
        {
            /* i is 0‑based index for a(i+1) */
            const seq_t prev = cache[i - 1];                /* a(i)   */
            const seq_t incr = ((seq_t)(i + 1)) * 2 - 1;    /* 2*(i+1)-1 = 2*i+1 */
            const seq_t sum  = prev + incr;
            cache[i] = noz(sum);
        }
        cached_len = new_len;
    }

    return cache[n - 1];
}
