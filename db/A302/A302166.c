/*
 * A302166  Number of 4Xn 0..1 arrays with every element equal to 0, 1 or 3
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 64, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A302166
   a(1)=8, a(2)=10, a(3)=17, a(4)=29, a(5)=51, a(6)=109,
   a(7)=206, a(8)=407, a(9)=791, a(10)=1584, a(11)=3104,
   a(12)=6165, a(13)=12131, a(14)=24059, a(15)=47443,
   a(16)=93958, a(17)=185498, a(18)=367087, a(19)=725156,
   a(20)=1434386, a(21)=2834565, a(22)=5605574, a(23)=11079494;
   for n>23:
   a(n)=a(n-1)+a(n-2)+8*a(n-4)-4*a(n-5)-4*a(n-6)-a(n-7)
        -22*a(n-8)+a(n-9)+5*a(n-10)+2*a(n-11)+28*a(n-12)+6*a(n-13)
        -3*a(n-14)-3*a(n-15)-17*a(n-16)-4*a(n-17)+2*a(n-18)
        +2*a(n-19)+4*a(n-20)-a(n-22)
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static inline seq_t mul_small(seq_t x, unsigned m)
{
    return x * (seq_t)m;
}

/* largest n we are prepared to compute (must be >= 1) */
#define MAX_N 20000

seq_t a(idx_t n)
{
    /* Return 0 for undefined indices (<1); defined for all n>=1 */
    if (n < 1)
        return 0;

    /* Dispatching large n: we support up to MAX_N */
    if (n > MAX_N) {
        /* Increase cap if needed; for simplicity we abort if n too large */
        /* In a production setting, you'd enlarge the array with new[] */
        abort();
    }

    /* static cache of computed terms, shared across calls */
    static seq_t cache[MAX_N + 1];
    /* Indicates how far we've computed (highest index filled, inclusive) */
    static size_t max_computed = 0;

    /* Initial values for 1..23 */
    static const seq_t init[24] = {
        (seq_t)0,  /* unused index 0 */
        (seq_t)8, (seq_t)10, (seq_t)17, (seq_t)29,
        (seq_t)51, (seq_t)109, (seq_t)206, (seq_t)407,
        (seq_t)791, (seq_t)1584, (seq_t)3104, (seq_t)6165,
        (seq_t)12131, (seq_t)24059, (seq_t)47443, (seq_t)93958,
        (seq_t)185498, (seq_t)367087, (seq_t)725156,
        (seq_t)1434386, (seq_t)2834565, (seq_t)5605574,
        (seq_t)11079494
    };

    /* Initialize cache if this is the first call */
    if (max_computed == 0) {
        for (size_t i = 0; i <= 23; ++i)
            cache[i] = init[i];
        max_computed = 23;
    }

    /* If requested n is already computed, just return it */
    if (n <= max_computed)
        return cache[n];

    /* Compute terms lazily up to n */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        seq_t pos_sum = cache[i-1] + cache[i-2] + mul_small(cache[i-4], 8)
                        + mul_small(cache[i-10], 5) + mul_small(cache[i-11], 2)
                        + mul_small(cache[i-12], 28) + mul_small(cache[i-13], 6)
                        + mul_small(cache[i-18], 2) + mul_small(cache[i-19], 2)
                        + mul_small(cache[i-20], 4) + cache[i-9];
        seq_t neg_sum = mul_small(cache[i-5], 4) + mul_small(cache[i-6], 4)
                        + cache[i-7] + mul_small(cache[i-8], 22)
                        + mul_small(cache[i-14], 3) + mul_small(cache[i-15], 3)
                        + mul_small(cache[i-16], 17) + mul_small(cache[i-17], 4)
                        + cache[i-22];
        cache[i] = pos_sum - neg_sum;
    }

    max_computed = (size_t)n;
    return cache[n];
}
