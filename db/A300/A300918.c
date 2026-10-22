/*
 * A300918  Number of nX3 0..1 arrays with every element equal to 0, 2, 3, 4
 *          or 5 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A300918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical linear recurrence (order 21):
   a(n) =  2*a(n-1) + 11*a(n-2) +   a(n-3) - 17*a(n-4) - 46*a(n-5) - 68*a(n-6)
          - 42*a(n-7) + 4*a(n-8) + 43*a(n-9) + 28*a(n-10)
          - 34*a(n-11) - 12*a(n-12) +   8*a(n-13) + 46*a(n-14)
          + 22*a(n-15) + 15*a(n-16) - 2*a(n-17) - 2*a(n-18) - 3*a(n-19)
          - 1*a(n-21)                         (no a(n-20) term)
   Indices start at 1.  Initial values:
   a(1)=1, a(2)=5, a(3)=20, a(4)=76, a(5)=290, a(6)=1245, a(7)=5160,
   a(8)=21819, a(9)=91713, a(10)=388059, a(11)=1639111, a(12)=6934811,
   a(13)=29326929, a(14)=124078694, a(15)=524899537, a(16)=2220794401,
   a(17)=9395650615, a(18)=39752075589, a(19)=168185729492,
   a(20)=711577787970, a(21)=3010611147958, a(22)=12737611709683 */
#include <stdlib.h>

static const seq_t init[22] = {
    1ULL, 5ULL, 20ULL, 76ULL, 290ULL, 1245ULL,
    5160ULL, 21819ULL, 91713ULL, 388059ULL,
    1639111ULL, 6934811ULL, 29326929ULL, 124078694ULL,
    524899537ULL, 2220794401ULL, 9395650615ULL,
    39752075589ULL, 168185729492ULL, 711577787970ULL,
    3010611147958ULL, 12737611709683ULL
};

static const int coeff[22] = {
    0,    /* unused */
    2,    11,     1,  -17,  -46,  -68,  -42,   4,   43,   28,
   -34,  -12,     8,   46,   22,   15,  -2,   -2,   -3,   0,
   -1    /* a(n-21) */
};

seq_t a(idx_t n)
{
    /* Return immediately for the first 22 terms. */
    if (n <= 22) {
        return init[n - 1];
    }

    /* Static cache for all computed terms. */
    static seq_t *cache = NULL;    /* array indexed from 1 */
    static idx_t cache_size = 0;   /* current allocated size (indexable up to cache_size-1) */
    static idx_t max_cached = 0;   /* highest index actually computed */

    /* Ensure the cache is large enough for indices 1..n. */
    if (cache_size <= n) {
        idx_t new_size = n + 1;                      /* +1 because we ignore index 0 */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – return 0 (unlikely in real tests). */
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise the first 22 terms if they are not yet cached. */
    if (max_cached < 22) {
        for (idx_t i = 1; i <= 22; ++i) {
            cache[i] = init[i - 1];
        }
        max_cached = 22;
    }

    /* Compute missing terms up to n. */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        /* Recurrence uses offsets 1..21 (offset 20 is absent). */
        for (int offset = 1; offset <= 21; ++offset) {
            int c = coeff[offset];
            if (c == 0) {
                continue;
            }
            idx_t idx = k - (idx_t)offset;
            seq_t term = cache[idx];
            if (c > 0) {
                sum += term * (seq_t)c;
            } else {
                sum -= term * (seq_t)(-c);
            }
        }
        cache[k] = sum;
    }
    max_cached = n;

    return cache[n];
}
