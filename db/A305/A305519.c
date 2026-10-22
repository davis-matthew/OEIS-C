/*
 * A305519  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4 or 5 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A305519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical linear recurrence of order 15:
 * a(1)=8
 * a(2)=128
 * a(3)=1482
 * a(4)=16480
 * a(5)=186237
 * a(6)=2102155
 * a(7)=23747613
 * a(8)=268359015
 * a(9)=3032800805
 * a(10)=34276734724
 * a(11)=387403233851
 * a(12)=4378561680838
 * a(13)=49488189296483
 * a(14)=559335597308662
 * a(15)=6321843211366946
 * a(16)=71452123170486735
 * a(17)=807581990796222475
 *
 * For n>17:
 *   a(n) = 10*a(n-1)+31*a(n-2)-117*a(n-3)-700*a(n-4)-714*a(n-5)
 *         +564*a(n-6)+2057*a(n-7)+1648*a(n-8)
 *         -216*a(n-9)-1318*a(n-10)-705*a(n-11)-6*a(n-12)
 *         -165*a(n-13)-278*a(n-14)-212*a(n-15);
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache up to the largest n computed so far */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* allocated slots (index 0 is unused) */
    static idx_t max_calculated = 0;       /* largest index whose value is known */

    /* Initial terms a(1) .. a(17), index 0 unused to keep 1‑based indexing */
    static const seq_t init[18] = {
        0ULL,            /* unused */
        8ULL,
        128ULL,
        1482ULL,
        16480ULL,
        186237ULL,
        2102155ULL,
        23747613ULL,
        268359015ULL,
        3032800805ULL,
        34276734724ULL,
        387403233851ULL,
        4378561680838ULL,
        49488189296483ULL,
        559335597308662ULL,
        6321843211366946ULL,
        71452123170486735ULL,
        807581990796222475ULL
    };

    /* Recurrence coefficients for a(n) in terms of a(n-1) .. a(n-15) */
    static const int coeff[15] = {
        10,   31,  -117, -700, -714,
        564, 2057, 1648,   -216, -1318,
        -705,   -6,  -165,  -278,  -212
    };

    /* Ensure the cache has slots for indices 1 .. n */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache && new_size > 0)
            abort();          /* memory exhausted */
        cache = new_cache;
        cache_size = new_size;
    }

    /* If we already know the value, return it immediately */
    if (n <= max_calculated)
        return cache[n];

    /* Compute values from max_calculated+1 up to n, storing them in the cache */
    for (idx_t k = max_calculated + 1; k <= n; ++k) {
        seq_t val;
        if (k <= 17) {
            val = init[(size_t)k];
        } else {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (int j = 0; j < 15; ++j) {
                seq_t term = cache[(size_t)(k - (idx_t)(j + 1))];
                int c = coeff[j];
                if (c >= 0)
                    sum_pos += (seq_t)c * term;
                else
                    sum_neg += (seq_t)(-c) * term;
            }
            val = sum_pos - sum_neg;
        }
        cache[(size_t)k] = val;
    }

    max_calculated = n;
    return cache[n];
}
