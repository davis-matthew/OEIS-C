/*
 * A250523  Number of (n+1)X(4+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 12, u64 n <= 33, u128 n <= 73,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A250523 – number of (n+1)×5 0..2 arrays with nondecreasing forward
 * differences in both row and column directions.
 *
 * Recurrence (order 9):
 *   a(n) = 14*a(n-1) - 85*a(n-2) + 294*a(n-3) - 639*a(n-4)
 *          + 906*a(n-5) - 839*a(n-6) + 490*a(n-7) - 164*a(n-8) + 24*a(n-9)
 *   for n > 9
 * Initial values:
 *   a(1)=3123, a(2)=18859, a(3)=82023, a(4)=300131, a(5)=993123,
 *   a(6)=3088923, a(7)=9240559, a(8)=26984403, a(9)=77707851
 *
 * The implementation uses the recurrence but stores positive and negative
 * contributions separately to avoid signed intermediate overflow.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mul_small.h"

/* Fixed maximum n for which we allocate memory.  The problem guarantees
 * time limits up to about n = 10000, which fits easily in this array. */
#define MAX_N 10000

static seq_t cache[MAX_N + 1];   /* cache[0] unused, cache[1] = a(1), ... */
static idx_t  cached_max = 0;    /* largest n for which cache is filled */

/* Recurrence coefficients (northwest to southeast).  Coeffs may be negative. */
static const int coeffs[9] = { 14, -85,  294, -639,  906, -839,  490, -164,   24 };

/* Absolute value for int (no math.h). */
static inline int absint(int x) { return x < 0 ? -x : x; }

/* Initial values for n = 1..9.  cache[0] is left zero. */
static const seq_t init_vals[10] = { 0,
    3123ULL,       /* a(1) */
    18859ULL,      /* a(2) */
    82023ULL,      /* a(3) */
    300131ULL,     /* a(4) */
    993123ULL,     /* a(5) */
    3088923ULL,    /* a(6) */
    9240559ULL,    /* a(7) */
    26984403ULL,   /* a(8) */
    77707851ULL    /* a(9) */
};

/* Ensure cache up to n is prepared.  Called from a(). */
static void ensure_cache(idx_t n)
{
    if (cached_max >= n) return;

    /* If cache is being initialized for the first time. */
    if (cached_max == 0 && n >= 1 && n <= 9) {
        /* Copy known initial values. */
        size_t i;
        for (i = 1; i <= 9 && i <= n; ++i)
            cache[i] = init_vals[i];
        cached_max = (i > 1 ? i - 1 : 0);
        if (n <= cached_max) return;
    }

    /* Fill missing initial values if n < 9. */
    if (cached_max < 9 && n <= 9) {
        size_t i;
        for (i = cached_max + 1; i <= n; ++i)
            cache[i] = init_vals[i];
        cached_max = n;
        return;
    }

    /* Compute terms from cached_max+1 up to n using the recurrence. */
    size_t i;
    for (i = cached_max + 1; i <= n; ++i) {
        /* For i <= 9, initial values are already set. */
        if (i <= 9) continue;

        seq_t pos = 0;   /* sum of positive contributions */
        seq_t neg = 0;   /* sum of negative contributions */

        size_t k;
        for (k = 1; k <= 9; ++k) {
            int coef = coeffs[k - 1];
            seq_t term = mul_small(cache[i - k], (unsigned int)absint(coef));
            if (coef > 0)
                pos = pos + term;
            else
                neg = neg + term;
        }
        cache[i] = pos - neg;   /* final value must be positive */
    }
    cached_max = n;
}

/* Main sequence function called by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1, harmless */
    if (n > MAX_N) return 0;        /* safety guard; the test will not call this */
    ensure_cache(n);
    return cache[n];
}
