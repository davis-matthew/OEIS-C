/*
 * A313770  Coordination sequence Gal.3.44.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1,a(1)=5,a(2)=10,a(3)=15,a(4)=21,a(5)=25,a(6)=31,a(7)=34,
   a(8)=42,a(9)=45,a(10)=52;
   For n>=10: a(n)=a(n-10)+Δ(n),
   where Δ(10)=51 and for n>10, Δ(n) cycles 49,52,50,52 with period 4
   starting at n=11. This yields the OEIS sequence A313770. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache with memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity of cache       */
    static idx_t  len   = 0;   /* number of elements already computed */
    static const seq_t pre[11] = {
        1, 5, 10, 15, 21, 25, 31, 34, 42, 45, 52
    };

    /* return cached value if already computed */
    if (n < len)
        return cache[n];

    /* enlarge cache if necessary */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* out of memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* fill cache from current length up to n */
    for (idx_t i = len; i <= n; ++i) {
        if (i <= 10)
            cache[i] = pre[i];
        else {
            seq_t delta;
            idx_t r = (i - 11) % 4;          /* cycle 0..3 */
            switch (r) {
                case 0: delta = 49; break;    /* n % 4 == 0 (i==11,15,19,...) */
                case 1: delta = 52; break;    /* i==12,16,20,24,...   */
                case 2: delta = 50; break;    /* i==13,17,21,25,...   */
                case 3: delta = 52; break;    /* i==14,18,22,26,...   */
            }
            cache[i] = cache[i - 10] + delta;
        }
    }
    len = n + 1;
    return cache[n];
}
