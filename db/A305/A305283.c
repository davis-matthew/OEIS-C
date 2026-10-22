/*
 * A305283  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3, 4
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 11*a(n-2) + 5*a(n-3) - 19*a(n-4) -23*a(n-5)
 *            -2*a(n-6) -14*a(n-7) -16*a(n-8) -19*a(n-9) -12*a(n-10) - a(n-11)
 *            -6*a(n-12) + 3*a(n-13) - a(n-14) + 2*a(n-15) for n > 16, with
 *            the first 16 terms: a(1)=1, a(2)=23, a(3)=55, a(4)=279,
 *            a(5)=895, a(6)=3696, a(7)=13289, a(8)=51734, a(9)=1924...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to handle arbitrary order of calls */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* initial values, index 0 unused */
    static const seq_t init[17] = {
        0, /* a(0) placeholder */
        1ULL,
        23ULL,
        55ULL,
        279ULL,
        895ULL,
        3696ULL,
        13289ULL,
        51734ULL,
        192418ULL,
        735241ULL,
        2765966ULL,
        10501378ULL,
        39658770ULL,
        150239389ULL,
        568125614ULL,
        2150618007ULL
    };

    if (n == 0)
        return 0;                 /* undefined for offset 0, return 0 */

    /* enlarge cache if necessary */
    if (n >= cap)
    {
        size_t newcap = n + 1;
        seq_t *newcache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!newcache)
            abort(); /* memory failure */

        /* copy existing values and initialize new ones if needed */
        for (size_t i = cap; i < newcap; ++i)
        {
            if (i <= 16)
                newcache[i] = init[i];
            else
                newcache[i] = 0; /* placeholder, will be computed later */
        }

        cache = newcache;
        cap = newcap;
    }

    if (n <= 16)
        return cache[n];

    /* coefficients for the recurrence */
    static const int coeff[15] = {
        1,  11,  5,
        -19, -23, -2,
        -14, -16, -19,
        -12, -1,  -6,
         3, -1,   2
    };

    /* compute terms up to n */
    for (idx_t k = 17; k <= n; ++k)
    {
        seq_t sum = 0;
        for (int i = 1; i <= 15; ++i)
        {
            uint64_t abs_coeff = coeff[i - 1] >= 0 ? (uint64_t)coeff[i - 1]
                                                   : (uint64_t)(-coeff[i - 1]);
            seq_t prod = cache[k - i] * (seq_t)abs_coeff;
            if (coeff[i - 1] >= 0)
                sum += prod;
            else
                sum -= prod;        /* unsigned subtraction; underflow does not occur */
        }
        cache[k] = sum;
    }

    return cache[n];
}
