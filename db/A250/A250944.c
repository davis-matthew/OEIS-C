/*
 * A250944  Number of (2+1)X(n+1) 0..2 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 29, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A250944 - number of (2+1)×(n+1) 0..2 arrays with the given monotonicity constraints.
    The sequence satisfies the linear recurrence
        a(n) = 10 a(n-1) - 29 a(n-2) - 6 a(n-3) + 136 a(n-4) - 90 a(n-5)
               - 217 a(n-6) + 186 a(n-7) + 143 a(n-8) - 124 a(n-9) - 32 a(n-10)
               + 24 a(n-11)
    for n ≥ 12, with initial values
        a(1)=125, a(2)=699, a(3)=3355, a(4)=15431, a(5)=67595,
        a(6)=287045, a(7)=1188599, a(8)=4827913, a(9)=19308213,
        a(10)=76254749, a(11)=298049037.
    This implementation uses dynamic memoisation to support any call order.

    seq_t and idx_t are supplied by the build system (unsigned integer types
    or arbitrary‑precision integer class).  Only the allowed operators and
    standard headers are used.
*/

#include <stddef.h>
#include <stdlib.h>

static const int coeff[11] = {
    10,  -29,  -6,  136, -90,  -217,  186,  143,  -124, -32, 24
};

/* initial values for a(1)..a(11) */
static const seq_t base[12] = {
    0,          /* a(0) unused, but convenient for 1‑based indexing */
    125,        /* a(1) */
    699,        /* a(2) */
    3355,       /* a(3) */
    15431,      /* a(4) */
    67595,      /* a(5) */
    287045,     /* a(6) */
    1188599,    /* a(7) */
    4827913,    /* a(8) */
    19308213,   /* a(9) */
    76254749,   /* a(10) */
    298049037   /* a(11) */
};

seq_t a(idx_t n)
{
    /* no answer for n < 1 */
    if (n < 1) return (seq_t)0;

    /* dynamic memoisation: cache[1..len-1] contains a(1) .. a(len-1) */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size */
    static size_t len = 0;   /* number of valid entries (len-1 is max stored index) */

    /* initialise cache with the first 11 terms if not yet done */
    if (len == 0) {
        size_t needed = 12;           /* we need indices 1..11 inclusive */
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        for (size_t i = 1; i < needed; ++i) {
            cache[i] = base[i];
        }
        len = needed;
        cap = needed;
    }

    /* ensure we have space for requested n */
    if ((size_t)n >= cap) {
        size_t newcap = cap > 0 ? cap * 2 : 16;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cap = newcap;
    }

    /* compute terms up to n if necessary */
    if ((size_t)n >= len) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            seq_t sum = 0;
            /* apply the 11‑term linear recurrence */
            for (int k = 1; k <= 11; ++k) {
                seq_t t = cache[i - k] * (seq_t)(k <= 11 ? coeff[k-1] >= 0 ? coeff[k-1] : -coeff[k-1] : 0);
                if (coeff[k-1] >= 0) {
                    sum += t;
                } else {
                    sum -= t;
                }
            }
            cache[i] = sum;
        }
        len = (size_t)n + 1;
    }

    return cache[n];
}
