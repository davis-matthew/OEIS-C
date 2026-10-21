/*
 * A260277  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000000 00000001 or 00001011.
 * https://oeis.org/A260277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) + a(n-4) + a(n-8) (for n > 10); Initial
 *            values: a(1)=34, a(2)=46, a(3)=99, a(4)=202, a(5)=366, a(6)=714,
 *            a(7)=1428, a(8)=2750, a(9)=5291, a(10)=10334
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 29, u64 n <= 62, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* 1‑based array of results   */
    static idx_t computed_upto = 0;      /* largest index already computed */
    static idx_t cap = 0;                /* current capacity of cache    */

    /* allow calls with n == 0 (not part of the sequence) */
    if (n == 0)
        return 0;

    /* ensure we have room for at least max(n,10) elements   */
    idx_t need = n;
    if (need < 10) need = 10;
    if (cap <= need) {
        idx_t new_cap = (cap == 0 ? 1 : cap);
        while (new_cap <= need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* first call: initialise the base values */
    if (computed_upto == 0) {
        cache[1] = 34;   cache[2] = 46;   cache[3] = 99;
        cache[4] = 202;  cache[5] = 366;  cache[6] = 714;
        cache[7] = 1428; cache[8] = 2750; cache[9] = 5291;
        cache[10] = 10334;
        computed_upto = 10;
    }

    /* compute missing terms up to 'need' (which is at least n) */
    for (idx_t i = computed_upto + 1; i <= need; ++i) {
        cache[i] = cache[i - 1] + (seq_t)3 * cache[i - 3]
                    + cache[i - 4] + cache[i - 8];
    }
    computed_upto = need;

    return cache[n];
}
