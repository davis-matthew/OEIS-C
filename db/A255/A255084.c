/*
 * A255084  Number of (n+2)X(1+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 1 and no antidiagonal sum 2 and no row sum 0 and no column sum
 *          0.
 * https://oeis.org/A255084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 20, u64 n <= 45, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (order 20) for A255084:
   For n > 23,
       a(n) =  a(n‑1)  +  a(n‑2)  + 4*a(n‑3)  + 5*a(n‑4)  + 5*a(n‑5)
            + a(n‑6)  – 14*a(n‑7) – 47*a(n‑8) – 17*a(n‑9) + 7*a(n‑10)
            + 29*a(n‑11)+ 17*a(n‑12)+26*a(n‑13)+21*a(n‑14)
            – 10*a(n‑15)+ 3*a(n‑16) – 2*a(n‑17) – 3*a(n‑18)
            + a(n‑19)  –  a(n‑20)
   Initial values (1‑based indexing): a(1)=108, a(2)=265, a(3)=623, a(4)=1483,
   a(5)=3658, a(6)=8830, a(7)=21442, a(8)=51939, a(9)=125961,
   a(10)=305879, a(11)=742809, a(12)=1803332, a(13)=4375230,
   a(14)=10620310, a(15)=25781121, a(16)=62583760, a(17)=151913810,
   a(18)=368742077, a(19)=895059804, a(20)=2172650028,
   a(21)=5273900506, a(22)=12801714866, a(23)=31074374291.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;          /* array of computed values */
    static size_t cache_len = 0;         /* number of values actually stored */
    static size_t cache_cap = 0;         /* allocated capacity */

    /* coefficients for recurrence, indices 0..19 correspond to n‑1..n‑20 */
    static const int coeff[20] = {
        1,  1,  4,  5,  5,  1, -14, -47, -17,   7,
       29, 17, 26, 21, -10,  3, -2, -3,  1,  -1
    };

    if (n == 0)
        return (seq_t)0;                 /* undefined, just return 0 */

    /* ensure cache has initial terms */
    if (cache_len == 0) {                /* first use: allocate and init */
        const unsigned long long init_val[23] = {
            108ULL, 265ULL, 623ULL, 1483ULL, 3658ULL,
            8830ULL, 21442ULL, 51939ULL, 125961ULL, 305879ULL,
            742809ULL, 1803332ULL, 4375230ULL, 10620310ULL,
            25781121ULL, 62583760ULL, 151913810ULL,
            368742077ULL, 895059804ULL, 2172650028ULL,
            5273900506ULL, 12801714866ULL, 31074374291ULL
        };
        const size_t init_len = 23;       /* we use the first 23 values */
        size_t cap = 64;
        if (cap < init_len)
            cap = init_len;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = cap;

        for (size_t i = 0; i < init_len; ++i)
            cache[i] = (seq_t)init_val[i];
        cache_len = init_len;
    }

    /* grow cache if needed */
    if ((size_t)n > cache_len) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n)
            new_cap = (new_cap == 0) ? 64 : new_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return known value if already computed */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* compute terms up to n using recurrence */
    for (size_t k = cache_len + 1; k <= (size_t)n; ++k) {
        seq_t sum_pos = (seq_t)0;
        seq_t sum_neg = (seq_t)0;
        for (int i = 0; i < 20; ++i) {
            size_t idx = k - (i + 1) - 1;      /* zero‑based index in cache */
            seq_t term = cache[idx];
            int c = coeff[i];
            if (c > 0)
                sum_pos += (seq_t)(term * (seq_t)c);
            else
                sum_neg += (seq_t)(term * (seq_t)(-c));
        }
        cache[k - 1] = sum_pos - sum_neg;
    }
    cache_len = n;
    return cache[n - 1];
}
