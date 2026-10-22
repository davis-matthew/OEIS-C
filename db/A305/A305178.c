/*
 * A305178  Number of n X 4 0..1 arrays with every element unequal to 1, 2, 5,
 *          6 or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+a(n-2)+3*a(n-3)-2*a(n-5)-5*a(n-6)-3*a(n-7)-2*a(n-9)+a(n-10)+2*a(n-11)
 *            * Initial values (offset 1): * a(1)=2, a(2)=1, a(3)=9, a(4)=10,
 *            a(5)=13, a(6)=47, a(7)=71, a(8)=118, a(9)=241, * a(10)=453,
 *            a(11)=870, a(12)=1608, a(13)=3110, a(14)=6075, a(15)=11491, *
 *            a(16)=22105, a(17)=42831, a...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 68, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – a(0) is undefined */

    static seq_t *cache   = NULL;
    static idx_t  capacity = 0;    /* number of elements allocated */
    static idx_t  max_done = 0;    /* largest index already computed */

    /* ensure storage is large enough for indices up to n */
    if (n >= capacity) {
        idx_t new_cap = capacity ? capacity * 2 : 32;
        if (new_cap <= n) new_cap = n + 1;
        if (new_cap < 19) new_cap = 19;           /* need room for the first 18 terms */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return 0;                       /* out of memory – return zero as a sane fallback */
        cache = tmp;
        capacity = new_cap;
    }

    /* initialise the first 18 terms once */
    if (max_done < 18) {
        if (max_done < 1) cache[0] = 0;           /* unused slot */
        cache[1] = 2;   cache[2] = 1;   cache[3] = 9;   cache[4] = 10;
        cache[5] = 13;  cache[6] = 47;  cache[7] = 71;  cache[8] = 118;
        cache[9] = 241; cache[10] = 453; cache[11] = 870; cache[12] = 1608;
        cache[13] = 3110; cache[14] = 6075; cache[15] = 11491;
        cache[16] = 22105; cache[17] = 42831; cache[18] = 82321;
        max_done = 18;
    }

    /* return already computed value */
    if (n <= max_done) return cache[n];

    /* compute terms up to n using the empirical recurrence (valid for n>18) */
    for (idx_t i = max_done + 1; i <= n; ++i) {
        if (i <= 18) continue;                 /* safety – base terms already set */

        seq_t term = cache[i-1] + cache[i-2] + 3 * cache[i-3];
        term = term - (2 * cache[i-5] + 5 * cache[i-6] +
                        3 * cache[i-7] + 2 * cache[i-9]);
        term = term + cache[i-10] + 2 * cache[i-11];
        cache[i] = term;
    }
    max_done = n;
    return cache[n];
}
