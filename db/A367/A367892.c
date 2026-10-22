/*
 * A367892  Total number of steps of Euclid's GCD algorithm to calculate
 *          gcd(x,y) for all pairs x,y in the range 1 <= y <= x <= n.
 * https://oeis.org/A367892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{x=1..n} Sum_{y=1..x} A107435(x,y) Recurrence: a(n) =
 *            a(n-1) + 1 + A049826(n) with a(0)=0. A107435(x,y) is the number
 *            of Euclid's algorithm steps for gcd(x,y)
 * Range:     u8 n <= 15, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoized cache for a(n).  a_cache[0] is 0. */
    static seq_t *a_cache = NULL;
    static idx_t max_n   = 0;          /* largest n already computed */
    static uint8_t *steps = NULL;      /* triangular array of step counts */

    if (n == (idx_t)0)
        return (seq_t)0;

    /* Expand tables if needed. */
    if (n > max_n) {
        idx_t new_max = n;

        /* Allocate or grow a_cache. */
        size_t new_a_bytes = sizeof(seq_t) * (size_t)(new_max + 1);
        seq_t *new_a_cache = (seq_t *)realloc(a_cache, new_a_bytes);
        if (!new_a_cache) return (seq_t)0;    /* allocation failed */
        a_cache = new_a_cache;
        a_cache[0] = (seq_t)0;               /* maintain a(0)=0 */

        /* Allocate or grow steps array. */
        size_t new_steps_bytes = sizeof(uint8_t) * (size_t)(new_max * (new_max + 1) / 2);
        uint8_t *new_steps = (uint8_t *)realloc(steps, new_steps_bytes);
        if (!new_steps) return (seq_t)0;
        steps = new_steps;

        /* Compute new rows of steps and a(n) for a = max_n+1 .. new_max */
        for (idx_t a = max_n + 1; a <= new_max; ++a) {
            seq_t sum_a = 0;
            idx_t row_start = a * (a - 1) / 2;   /* index offset for row a */

            for (idx_t y = 1; y <= a; ++y) {
                idx_t idx = row_start + (y - 1);   /* linear index for (a,y) */

                if (y == a) {                     /* gcd(a,a) */
                    steps[idx] = 1;
                    sum_a += (seq_t)1;
                } else {
                    idx_t r = a % y;
                    if (r == 0) {                  /* remainder zero */
                        steps[idx] = 1;
                        sum_a += (seq_t)1;
                    } else {
                        /* Steps for (y, r) where y > r. */
                        idx_t idx_prev = y * (y - 1) / 2 + (r - 1);
                        uint8_t prev = steps[idx_prev];
                        steps[idx] = 1 + prev;
                        sum_a += (seq_t)(1 + prev);
                    }
                }
            }
            a_cache[a] = a_cache[a - 1] + sum_a;
        }
        max_n = new_max;
    }

    return a_cache[n];
}
