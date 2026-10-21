/*
 * A275053  G.f.: 3F2([1/9, 2/9, 7/9], [2/3,1], 729x).
 * https://oeis.org/A275053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n+1)=a(n)*3*(2+9*n)*(1+9*n)*(7+9*n)/((n+1)^2*(3*n+2))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* local Euclidean gcd for unsigned seq_t */
static inline seq_t seq_gcd_local(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Ensure that @arr has capacity >= @needed */
static void ensure_capacity_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t newcap = (*cap > 0) ? (*cap << 1) : 4;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *arr = tmp;
        *cap = newcap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n < (idx_t)cache_len) {
        return cache[n];
    }

    /* grow cache to hold up to n */
    ensure_capacity_seq(&cache, &cache_cap, (size_t)(n + 1));

    for (idx_t k = (idx_t)cache_len; k <= n; ++k) {
        seq_t val;
        if (k == 0) {
            val = 1;
        } else {
            seq_t prev = cache[k - 1];

            /* numerators and denominators of the recurrence */
            seq_t num[4];
            seq_t den[3];

            num[0] = 3;
            num[1] = (seq_t)(2 + 9 * (k - 1));
            num[2] = (seq_t)(1 + 9 * (k - 1));
            num[3] = (seq_t)(7 + 9 * (k - 1));

            den[0] = (seq_t)(k);      /* (k) == (n+1) for current k */
            den[1] = (seq_t)(k);
            den[2] = (seq_t)(3 * (k - 1) + 2);

            /* cancel common factors */
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 3; ++j) {
                    seq_t g = seq_gcd_local(num[i], den[j]);
                    if (g > 1) {
                        num[i] /= g;
                        den[j] /= g;
                    }
                }
            }

            /* multiply remaining numerators with prev */
            seq_t prod = prev;
            for (int i = 0; i < 4; ++i) {
                prod *= num[i];
            }

            /* divide by remaining denominators */
            for (int j = 0; j < 3; ++j) {
                prod /= den[j];
            }

            val = prod;
        }

        cache[k] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
