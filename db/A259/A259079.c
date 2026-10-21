/*
 * A259079  Octagonal numbers (A000567) that are other octagonal numbers
 *          divided by 2.
 * https://oeis.org/A259079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1331715*a(n-1) - 1331715*a(n-2) + a(n-3) for n > 3
 *            Initial terms: a(1)=280, a(2)=373212840, a(3)=497012764340408
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t capacity = 0;
static idx_t cache_size = 0;

static void ensure_capacity(idx_t needed)
{
    if (needed <= capacity) return;
    size_t newcap = capacity ? capacity * 2 : 4;
    while (newcap < (size_t)needed) newcap *= 2;

    if (cache == NULL)
        cache = (seq_t *)malloc(newcap * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));

    if (cache == NULL) abort();   /* allocation failure */
    capacity = newcap;
}

static void init_initials(void)
{
    ensure_capacity(3);
    cache[0] = (seq_t)280;
    cache[1] = (seq_t)373212840;
    cache[2] = (seq_t)497012764340408ULL;
    cache_size = 3;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    if (cache_size == 0) init_initials();

    if (n <= cache_size) return cache[n - 1];

    /* Ensure storage up to n */
    ensure_capacity(n);

    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)280;
        } else if (i == 2) {
            val = (seq_t)373212840;
        } else if (i == 3) {
            val = (seq_t)497012764340408ULL;
        } else {
            /* a(i) = 1331715*a(i-1) - 1331715*a(i-2) + a(i-3) */
            val = (seq_t)1331715 * cache[i - 2]
                - (seq_t)1331715 * cache[i - 3]
                + cache[i - 4];
        }
        cache[i - 1] = val;
    }
    cache_size = n;
    return cache[n - 1];
}
