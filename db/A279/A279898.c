/*
 * A279898  Number of nX4 0..2 arrays with no element unequal to a strict
 *          majority of its king-move neighbors, with the exception of exactly
 *          one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A279898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A279898: Number of n×4 0..2 arrays with no element unequal to a strict
   majority of its king-move neighbors, with the exception of exactly one
   element, and with new values introduced in order 0 sequentially upwards.
   Recurrence (valid for n > 23):
       a(n) =  4*a(n‑1) + 4*a(n‑2) –20*a(n‑3) –26*a(n‑4) +44*a(n‑5)
             +74*a(n‑6) +18*a(n‑7) –25*a(n‑8) –176*a(n‑9) –246*a(n‑10)
             +82*a(n‑11)+331*a(n‑12)+102*a(n‑13)+91*a(n‑14) –88*a(n‑15)
             –360*a(n‑16)+80*a(n‑17)+140*a(n‑18) –16*a(n‑19) –16*a(n‑20)
   Initial values:
       a(1)=2, a(2)=8, a(3)=20, a(4)=56, a(5)=156, a(6)=428,
       a(7)=1208, a(8)=3316, a(9)=9168, a(10)=25200, a(11)=69152,
       a(12)=189996, a(13)=520700, a(14)=1426696, a(15)=3905118,
       a(16)=10677540, a(17)=29172966, a(18)=79621084, a(19)=217117820,
       a(20)=591513132, a(21)=1610070116, a(22)=4378827488,
       a(23)=11898970622.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* 16‑bit or 32‑bit values: no special handling needed.  64‑bit values: the
   recurrence's intermediate sums fit inside 2^64 for all n whose result
   fits in 64 bits, so the unsigned arithmetic is exact.  `seq_t` is
   an arbitrary‑precision integer type in the C++ build, so the same
   expression works there. */

static const seq_t init[24] = {
    0ULL, /* unused index 0 */
    2ULL, 8ULL, 20ULL, 56ULL, 156ULL, 428ULL, 1208ULL,
    3316ULL, 9168ULL, 25200ULL, 69152ULL, 189996ULL,
    520700ULL, 1426696ULL, 3905118ULL, 10677540ULL,
    29172966ULL, 79621084ULL, 217117820ULL, 591513132ULL,
    1610070116ULL, 4378827488ULL, 11898970622ULL
};

/* coefficients for the recurrence a(n) = sum_{k=1}^{20} c[k]*a(n-k) */
static const int coeff[20] = {
    4, 4, -20, -26, 44, 74, 18, -25, -176, -246,
    82, 331, 102, 91, -88, -360, 80, 140, -16, -16
};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* offset is 1, below that value is undefined */

    /* Static cache and its metadata */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* number of allocated elements */
    static idx_t last = 0;          /* largest index for which cache[*] is valid */

    /* Ensure the cache is large enough for index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                  /* allocation failed – return 0 */
            if (cache)
                free(cache);
            cache = NULL;
            cap = 0;
            last = 0;
            return 0;
        }
        cache = tmp;
        cap = new_cap;
        /* Initialise new slots to zero (optional for correctness, but safer) */
        for (size_t i = last + 1; i < cap; ++i)
            cache[i] = (seq_t)0;
    }

    /* Pre‑populate the cache with the known initial 23 values if not already done */
    if (last < 23) {
        idx_t limit = 23;
        if (n < 23) limit = n;
        for (idx_t i = last + 1; i <= limit; ++i)
            cache[i] = init[(size_t)i];
        if (limit > last)
            last = limit;
    }

    /* If the requested term is already computed, return it */
    if (n <= last)
        return cache[(size_t)n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 23) {          /* initial values already set */
            cache[(size_t)i] = init[(size_t)i];
            continue;
        }
        /* Compute a(i) = sum_{k=1}^{20} coeff[k-1] * a(i-k) */
        seq_t sum = (seq_t)0;
        for (int k = 0; k < 20; ++k) {
            idx_t j = i - (k + 1);
            const seq_t term = cache[(size_t)j];
            const int c = coeff[k];
            if (c >= 0)
                sum += term * (seq_t)c;
            else
                sum -= term * (seq_t)(-c);
        }
        cache[(size_t)i] = sum;
    }
    last = n;
    return cache[(size_t)n];
}
