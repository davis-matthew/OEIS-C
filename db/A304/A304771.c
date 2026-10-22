/*
 * A304771  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2, 3
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* AEIS A304771: Number of n×4 0..1 arrays with the king‑move
   restriction described in the OEIS entry.
   Recurrence (for n>13):
       a(n) = a(n-1)+a(n-2)+6*a(n-3)+3*a(n-4)+a(n-5)
              -3*a(n-6)-4*a(n-7)-4*a(n-8)-2*a(n-9)+2*a(n-10)
   Initial values:
       a(1)=8, a(2)=49, a(3)=67, a(4)=172, a(5)=443,
       a(6)=1094, a(7)=2710, a(8)=6861, a(9)=17205,
       a(10)=43090, a(11)=108354, a(12)=272151, a(13)=683041
*/

#include <stddef.h>
#include <stdlib.h>

/* Internal memoisation cache */
static seq_t *cache = NULL;          /* array of cached terms */
static idx_t cache_size = 0;         /* highest index computed so far */

/* Initialise the cache with the first 13 terms. */
static void init_cache(void)
{
    const size_t init_size = 13;
    cache = (seq_t *)calloc(init_size + 1, sizeof(seq_t));
    if (!cache) return;           /* allocation failed – stays NULL */

    cache[1]  = (seq_t)8;
    cache[2]  = (seq_t)49;
    cache[3]  = (seq_t)67;
    cache[4]  = (seq_t)172;
    cache[5]  = (seq_t)443;
    cache[6]  = (seq_t)1094;
    cache[7]  = (seq_t)2710;
    cache[8]  = (seq_t)6861;
    cache[9]  = (seq_t)17205;
    cache[10] = (seq_t)43090;
    cache[11] = (seq_t)108354;
    cache[12] = (seq_t)272151;
    cache[13] = (seq_t)683041;
    cache_size = init_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* outside the offset */

    /* Ensure cache is ready */
    if (!cache) init_cache();
    if (!cache) return (seq_t)0;           /* allocation failure */

    /* Return pre‑computed value if already known */
    if (n <= cache_size) return cache[n];

    /* Extend cache if needed */
    if ((size_t)n >= cache_size + 1) {
        size_t new_cap = (size_t)n + 1;    /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure */
        /* Initialise new slots to 0 */
        for (size_t i = cache_size + 1; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t v1  = cache[i-1];
        seq_t v2  = cache[i-2];
        seq_t v3  = cache[i-3];
        seq_t v4  = cache[i-4];
        seq_t v5  = cache[i-5];
        seq_t v6  = cache[i-6];
        seq_t v7  = cache[i-7];
        seq_t v8  = cache[i-8];
        seq_t v9  = cache[i-9];
        seq_t v10 = cache[i-10];
        cache[i] = v1 + v2 + (seq_t)6 * v3 + (seq_t)3 * v4 + v5
                   - (seq_t)3 * v6 - (seq_t)4 * v7
                   - (seq_t)4 * v8 - (seq_t)2 * v9 + (seq_t)2 * v10;
    }

    cache_size = n;
    return cache[n];
}
