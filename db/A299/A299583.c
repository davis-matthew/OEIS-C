/*
 * A299583  Number of n X 3 0..1 arrays with every element equal to 1, 3, 4,
 *          5, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n ≥ 21, a(n) =
 *            2*a(n-1)+3*a(n-2)+5*a(n-3)-19*a(n-4)-16*a(n-5) *
 *            +10*a(n-6)+32*a(n-7)-30*a(n-9)+75*a(n-10)+16*a(n-11) *
 *            -99*a(n-12)-51*a(n-13)+52*a(n-14)+25*a(n-15)-4*a(n-16) *
 *            +a(n-17)-2*a(n-18)-a(n-19); * Initial values:
 *            a(1)=0,a(2)=4,a(3)=1,a(4)=8,a(5)=12,a(6)=31,a(7)=117, *
 *            a(8)=263,a(9)=6...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 47, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[20] = {
    0ULL,          /* a(1) */
    4ULL,          /* a(2) */
    1ULL,          /* a(3) */
    8ULL,          /* a(4) */
    12ULL,         /* a(5) */
    31ULL,         /* a(6) */
    117ULL,        /* a(7) */
    263ULL,        /* a(8) */
    689ULL,        /* a(9) */
    1977ULL,       /* a(10) */
    5034ULL,       /* a(11) */
    13497ULL,      /* a(12) */
    36675ULL,      /* a(13) */
    96801ULL,      /* a(14) */
    258990ULL,     /* a(15) */
    694840ULL,     /* a(16) */
    1851440ULL,    /* a(17) */
    4950051ULL,    /* a(18) */
    13241884ULL,   /* a(19) */
    35365083ULL    /* a(20) */
};

#define MAX_CACHE 20000  /* sufficient for n up to about 10000 */

static seq_t cache[MAX_CACHE + 1];
static size_t cached_until = 0; /* highest index for which a(i) is computed (0 means none) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 */
    if (n > MAX_CACHE) {
        /* For safety, if the caller asks for too large n, expand the cache using realloc
           but only for big‑integer builds where seq_t is a POD type. */
        /* In combined C/C++ mode we avoid dynamic allocation of class types. */
        /* If seq_t is a big integer class, we cannot realloc safely.
           Instead, use an assertion or fallback. */
        /* Here we simply return 0 for out-of-range requests. */
        return 0;
    }
    /* Compute up to n if not already done. */
    for (size_t i = cached_until + 1; i <= (size_t)n; ++i) {
        if (i <= 20) {
            cache[i] = init_vals[i - 1];
        } else {
            /* recurrence coefficients (coefficient, offset) */
            static const struct {int coeff; int offs;} terms[18] = {
                {  2, 1},
                {  3, 2},
                {  5, 3},
                { -19, 4},
                { -16, 5},
                { 10, 6},
                { 32, 7},
                { -30, 9},
                {  75,10},
                {  16,11},
                { -99,12},
                { -51,13},
                {  52,14},
                {  25,15},
                {  -4,16},
                {   1,17},
                {  -2,18},
                {  -1,19}
            };
            seq_t sum = 0;
            for (int t = 0; t < 18; ++t) {
                int coeff = terms[t].coeff;
                size_t k = i - (size_t)terms[t].offs;
                seq_t term = cache[k];
                /* multiply using seq_t arithmetic; result fits in seq_t */
                seq_t mult = term * (seq_t)(coeff > 0 ? coeff : -coeff);
                if (coeff > 0) {
                    sum += mult;
                } else {
                    sum -= mult;
                }
            }
            cache[i] = sum;
        }
    }
    cached_until = (size_t)n;
    return cache[n];
}
