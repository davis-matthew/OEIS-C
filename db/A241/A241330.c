/*
 * A241330  Number of 3Xn 0..3 arrays with no element equal to fewer vertical
 *          neighbors than horizontal neighbors, with new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A241330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*a(n-1) -26*a(n-2) -530*a(n-3) +271*a(n-4) +1431*a(n-5)
 *            +437*a(n-6) +157*a(n-7) -438*a(n-8) -846*a(n-9) -243*a(n-10)
 *            -243*a(n-11) Initial values: a(1)=5, a(2)=85, a(3)=2344,
 *            a(4)=68935, a(5)=2034543, a(6)=60066019, a(7)=1773370241,
 *            a(8)=52356524981, a(9)=1545760698649, a(10)=456366455540...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n)
{
    /* n >= 1 */
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    size_t target = (size_t)n;          /* number of terms needed */

    /* ensure capacity */
    if (cap < target) {
        size_t newcap = cap ? cap : 1;
        while (newcap < target) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – unlikely */
        /* initialise new slots to 0 (not strictly needed) */
        for (size_t i = len; i < newcap; ++i) tmp[i] = 0;
        cache = tmp;
        cap = newcap;
    }

    /* initialise first 13 terms if necessary */
    if (len < 13) {
        const seq_t init[13] = {
            (seq_t)5,
            (seq_t)85,
            (seq_t)2344,
            (seq_t)68935,
            (seq_t)2034543,
            (seq_t)60066019,
            (seq_t)1773370241,
            (seq_t)52356524981,
            (seq_t)1545760698649,
            (seq_t)45636645554004,
            (seq_t)1347364712395962,
            (seq_t)39779252976746762,
            (seq_t)1174432544392935470
        };
        size_t i;
        for (i = len; i < 13 && i < target; ++i)
            cache[i] = init[i];
        len = i;
    }

    /* compute remaining terms using the recurrence */
    for (size_t idx = len; idx < target; ++idx) {
        /* positive part of recurrence */
        seq_t sum_pos = 0;
        sum_pos += (seq_t)31  * cache[idx-1];
        sum_pos += (seq_t)271  * cache[idx-4];
        sum_pos += (seq_t)1431 * cache[idx-5];
        sum_pos += (seq_t)437  * cache[idx-6];
        sum_pos += (seq_t)157  * cache[idx-7];

        /* negative part of recurrence */
        seq_t sum_neg = 0;
        sum_neg += (seq_t)26   * cache[idx-2];
        sum_neg += (seq_t)530  * cache[idx-3];
        sum_neg += (seq_t)438  * cache[idx-8];
        sum_neg += (seq_t)846  * cache[idx-9];
        sum_neg += (seq_t)243  * cache[idx-10];
        sum_neg += (seq_t)243  * cache[idx-11];

        /* final value: positive minus negative (non‑negative by definition) */
        cache[idx] = sum_pos - sum_neg;
    }
    len = target;
    return cache[target-1];
}
