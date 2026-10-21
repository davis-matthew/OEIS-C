/*
 * A251254  Number of (n+1)X(4+1) 0..1 arrays with every 2X2 subblock having a
 *          single 1 or two 1s on the same edge.
 * https://oeis.org/A251254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            5*a(n-1)+21*a(n-2)-115*a(n-3)-95*a(n-4)+706*a(n-5)+69*a(n-6)
 *            -1492*a(n-7)+208*a(n-8)+987*a(n-9)-228*a(n-10)-111*a(n-11)+28*a(n-12)
 *            for n >= 13, with initial values a(1)=84, a(2)=446, a(3)=1770,
 *            a(4)=9130, a(5)=38332, a(6)=192442, a(7)=836546, a(8)=4112126,
 *            a(9)=18298862, a(10)=88585834, a(...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* cache[0] unused, cache[1] = a(1) ... */
static idx_t  cache_len = 0;
static idx_t  cache_cap = 0;

static const seq_t init_vals[12] = {
    (seq_t)84ULL, (seq_t)446ULL, (seq_t)1770ULL, (seq_t)9130ULL, (seq_t)38332ULL,
    (seq_t)192442ULL, (seq_t)836546ULL, (seq_t)4112126ULL, (seq_t)18298862ULL,
    (seq_t)88585834ULL, (seq_t)400512170ULL, (seq_t)1918066656ULL
};

seq_t a(idx_t n)
{
    if (n < 1ULL) return (seq_t)0;

    /* Allocate cache on first call */
    if (!cache) {
        cache_cap = n > 12 ? n : 12;
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        for (idx_t i = 1; i <= 12; ++i) cache[i] = init_vals[i - 1];
        cache_len = 12;
    }

    /* Grow capacity if needed */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n) new_cap = n;
        cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing values */
    if (n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t pos = cache[i - 1] * 5 + cache[i - 2] * 21
                       + cache[i - 5] * 706 + cache[i - 6] * 69
                       + cache[i - 8] * 208 + cache[i - 9] * 987
                       + cache[i - 12] * 28;
            seq_t neg = cache[i - 3] * 115 + cache[i - 4] * 95
                       + cache[i - 7] * 1492 + cache[i - 10] * 228
                       + cache[i - 11] * 111;
            cache[i] = pos - neg;
        }
        cache_len = n;
    }

    return cache[n];
}
