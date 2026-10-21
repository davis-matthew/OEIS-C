/*
 * A280212  Number of nX2 0..2 arrays with no element unequal to a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors,
 *          with the exception of exactly two elements, and with new values
 *          introduced in order 0 sequentially upwards.
 * https://oeis.org/A280212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280212
   Empirical linear recurrence of order 10:
     a(n) = 4*a(n-1) + 6*a(n-2) - 20*a(n-3) - 49*a(n-4) + 24*a(n-5)
            + 148*a(n-6) + 128*a(n-7) - 48*a(n-8) - 128*a(n-9) - 64*a(n-10)
   for n > 14, with initial values:
     a(1)=1, a(2)=2, a(3)=26, a(4)=92, a(5)=307, a(6)=1072,
     a(7)=3541, a(8)=11834, a(9)=38687, a(10)=125380,
     a(11)=403369, a(12)=1286926, a(13)=4082099, a(14)=12875608
*/

#include <stdlib.h> /* malloc, realloc */
#include <stddef.h> /* size_t */

static seq_t *cache = NULL;          /* array of computed terms, index from 1 */
static idx_t  cache_len = 0;         /* greatest n already computed */
static size_t cache_size = 0;        /* allocated size of cache */

static void init_cache(void)
{
    /* allocate a minimal cache of 15 elements (indices 0..14) */
    const size_t init_size = 15;
    cache = (seq_t *)malloc(init_size * sizeof(seq_t));
    cache_size = init_size;
    /* base values, index 0 is unused */
    cache[1] = 1;
    cache[2] = 2;
    cache[3] = 26;
    cache[4] = 92;
    cache[5] = 307;
    cache[6] = 1072;
    cache[7] = 3541;
    cache[8] = 11834;
    cache[9] = 38687;
    cache[10] = 125380;
    cache[11] = 403369;
    cache[12] = 1286926;
    cache[13] = 4082099;
    cache[14] = 12875608;
    cache_len = 14;
}

static void grow_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;
    size_t new_size = (cache_size + 1024);
    if (new_size < n + 1)
        new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – terminate with a safe value (0) */
        exit(1);
    }
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1) {
        /* undefined for n < 1; return 0 as a safe value */
        return (seq_t)0;
    }

    /* initialise cache on first call */
    if (cache == NULL)
        init_cache();

    /* grow cache if needed */
    if (n + 1 > cache_size)
        grow_cache(n);

    /* return stored value if already computed */
    if (n <= cache_len)
        return cache[n];

    /* compute remaining terms up to n using recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t pos = 0;
        pos += 4 * cache[i - 1];
        pos += 6 * cache[i - 2];
        pos += 24 * cache[i - 5];
        pos += 148 * cache[i - 6];
        pos += 128 * cache[i - 7];

        seq_t neg = 0;
        neg += 20 * cache[i - 3];
        neg += 49 * cache[i - 4];
        neg += 48 * cache[i - 8];
        neg += 128 * cache[i - 9];
        neg += 64 * cache[i - 10];

        seq_t next = pos;
        next -= neg;                  /* safe: pos > neg for all n > 14 */
        cache[i] = next;
    }
    cache_len = n;
    return cache[n];
}
