/*
 * A299055  Number of nX3 0..1 arrays with every element equal to 0, 2, 3, 4,
 *          5 or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1)+9*a(n-2)-31*a(n-3)-62*a(n-4)+46*a(n-5)
 *            +78*a(n-6)-36*a(n-7)+53*a(n-8)+101*a(n-9)-86*a(n-10)
 *            -14*a(n-11)-26*a(n-12)-28*a(n-13)+6*a(n-14) for n>16 Initial
 *            values:
 *            a(1)=1,a(2)=13,a(3)=38,a(4)=199,a(5)=864,a(6)=4366,a(7)=21804,
 *            a(8)=111861,a(9)=578509,a(10)=3007390,a(11)=15688017, a(1...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int coeff[14] = { 5,  9, -31, -62,  46,  78, -36, 53, 101, -86, -14, -26, -28,  6 };
static const seq_t init[16] = { 1ULL, 13ULL, 38ULL, 199ULL, 864ULL, 4366ULL,
                                21804ULL, 111861ULL, 578509ULL, 3007390ULL,
                                15688017ULL, 81963177ULL, 428662899ULL, 2243048643ULL,
                                11740648591ULL, 61463549876ULL };
static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined, but guard */
    size_t target = (size_t)n;
    if (!cache) {
        cache = (seq_t*)malloc(16 * sizeof(seq_t));
        for (size_t i = 0; i < 16; ++i) cache[i] = init[i];
        cache_len = 16;
    }
    if (target <= cache_len) return cache[target - 1];

    /* Extend cache up to target */
    cache = (seq_t*)realloc(cache, target * sizeof(seq_t));
    for (size_t idx = cache_len; idx < target; ++idx) {
        seq_t pos = 0;
        seq_t neg = 0;
        /* idx corresponds to a(idx+1) (0‑based) */
        for (int k = 1; k <= 14; ++k) {
            seq_t term = cache[idx - k];
            int c = coeff[k - 1];
            if (c >= 0) {
                pos += term * (seq_t)c;
            } else {
                neg += term * (seq_t)(-c);
            }
        }
        cache[idx] = pos - neg;
    }
    cache_len = target;
    return cache[target - 1];
}
