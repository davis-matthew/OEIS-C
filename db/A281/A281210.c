/*
 * A281210  Number of 6 X n 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 4*a(n-4) - a(n-6) (for
 *            n>12) a(1)=0, a(2)=420, a(3)=1120, a(4)=1588, a(5)=2602,
 *            a(6)=4248, a(7)=6838, a(8)=11010, a(9)=17840, a(10)=29120,
 *            a(11)=47838, a(12)=78978
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 33, u64 n <= 78, u128 n <= 169,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"  /* alloc_seq_array */
#include "../utils/grow_seq.h"          /* grow_seq */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for non‑offset calls */

    static seq_t *cache = NULL;   /* array of values, index 0 unused */
    static size_t cache_size = 0;
    static idx_t max_cached = 0;

    /* initialise */
    if (!cache) {
        cache_size = 13;                     /* store indices 0..12 */
        cache = alloc_seq_array(cache_size); /* allocate array */
        /* base values 1..12 */
        cache[1]  = (seq_t)0;
        cache[2]  = (seq_t)420;
        cache[3]  = (seq_t)1120;
        cache[4]  = (seq_t)1588;
        cache[5]  = (seq_t)2602;
        cache[6]  = (seq_t)4248;
        cache[7]  = (seq_t)6838;
        cache[8]  = (seq_t)11010;
        cache[9]  = (seq_t)17840;
        cache[10] = (seq_t)29120;
        cache[11] = (seq_t)47838;
        cache[12] = (seq_t)78978;
        max_cached = 12;
    }

    /* grow cache if necessary */
    if ((idx_t)cache_size <= n) {
        size_t new_cap = cache_size * 2;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        grow_seq(&cache, &cache_size, new_cap);
    }

    /* return already computed value */
    if (n <= max_cached) return cache[n];

    /* compute values up to n using the recurrence */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        seq_t term1 = (seq_t)4 * cache[k - 1];
        seq_t term4 = (seq_t)4 * cache[k - 4];
        seq_t sum   = term1 + term4;            /* >= subtraction part */

        seq_t term2 = (seq_t)4 * cache[k - 2];
        seq_t term3 = (seq_t)2 * cache[k - 3];
        seq_t term5 = cache[k - 6];

        cache[k] = sum - (term2 + term3 + term5);
    }
    max_cached = n;
    return cache[n];
}
