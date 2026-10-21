/*
 * A274759  Modified quadranacci series.
 * https://oeis.org/A274759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 53, u32 n <= 104, u64 n <= 205,
 *            u128 n <= 409, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive definition for A274759 (Modified quadranacci):
   a(0)=0,  a(1)=1,  a(2)=1,  a(3)=1,  a(4)=1,  a(5)=2,
   a(6)=2,  a(7)=2,  a(8)=3,  a(9)=4,  a(10)=4, a(11)=6.
   For n>=12:
     if n%3==0: a(n)=a(n-3)+a(n-6)+a(n-9)+a(n-12);
     if n%3==1: a(n)=a(n-3)+a(n-4);
     if n%3==2: a(n)=a(n-1)+a(n-4);
*/

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoisation buffer           */
static size_t cache_cap = 0;          /* size of allocated buffer     */
static idx_t cache_max = (idx_t)-1;   /* largest index currently set  */

/* Ensure the cache holds indices 0 … n (inclusive) */
static void ensure_cache_capacity(idx_t n)
{
    size_t need = (n >= 11) ? (size_t)n + 1U : (size_t)12U;
    if (cache == NULL || need > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        cache = tmp;
        cache_cap = need;
    }
}

/* Initialise the known base values (indices 0 … 11) */
static void init_base_values(void)
{
    cache[0]  = (seq_t)0;
    cache[1]  = (seq_t)1;
    cache[2]  = (seq_t)1;
    cache[3]  = (seq_t)1;
    cache[4]  = (seq_t)1;
    cache[5]  = (seq_t)2;
    cache[6]  = (seq_t)2;
    cache[7]  = (seq_t)2;
    cache[8]  = (seq_t)3;
    cache[9]  = (seq_t)4;
    cache[10] = (seq_t)4;
    cache[11] = (seq_t)6;
    cache_max = 11;
}

/* Main function requested by the problem statement */
seq_t a(idx_t n)
{
    ensure_cache_capacity(n);

    if (cache_max == (idx_t)-1)
        init_base_values();

    if (n <= cache_max)
        return cache[n];

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        switch (i % 3) {
            case 0:  /* n divisible by 3 */
                if (i >= 12)
                    cache[i] = cache[i-3] + cache[i-6] + cache[i-9] + cache[i-12];
                break;
            case 1:  /* n%3 == 1, i >= 4 */
                cache[i] = cache[i-3] + cache[i-4];
                break;
            case 2:  /* n%3 == 2 */
                cache[i] = cache[i-1] + cache[i-4];
                break;
        }
    }

    cache_max = n;
    return cache[n];
}
