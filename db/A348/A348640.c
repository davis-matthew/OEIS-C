/*
 * A348640  Denominators of the remainders in the greedy Egyptian fraction
 *          representation of 1 with square denominators (A348626).
 * https://oeis.org/A348640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of 1 - Sum_{k=1..n} 1/A348626(k)^2 *
 *            (a(0)=1). The sequence is obtained by the greedy algorithm that,
 *            * at each step, subtracts the largest unit fraction with a
 *            square * denominator that is strictly smaller than the current
 *            remainder. * This function memoises the results, so re...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   ensure_capacity, floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"
#include "../utils/ensure_capacity.h"

/* Return a(n) */
seq_t a(idx_t n)
{
    /* Memoisation state */
    static seq_t *denoms   = NULL;   /* a(0..size-1) */
    static idx_t size      = 0;      /* number of terms already computed */
    static size_t cap      = 0;      /* capacity of the array */
    static seq_t r_num = 0, r_den = 0;  /* current remainder numerator/denominator */

    /* Initialise for a(0) */
    if (size == 0) {
        ensure_capacity(1, &cap, (void **)&denoms, sizeof(seq_t));
        r_num = 1;
        r_den = 1;
        denoms[0] = r_den;   /* a(0) = 1 */
        size = 1;
    }

    /* Compute up to n if necessary */
    if (n >= size) {
        /* Ensure the array can hold n+1 entries */
        ensure_capacity((size_t)n + 1, &cap, (void **)&denoms, sizeof(seq_t));

        for (idx_t i = size; i <= n; ++i) {
            /* Find smallest integer m >= 2 such that 1/m^2 < r_num/r_den
             * That is m^2 * r_num > r_den.
             */
            seq_t bound = floor_sqrt(r_den) + 1;    /* upper bound on m */
            if (bound < (seq_t)2) bound = (seq_t)2;
            seq_t low  = (seq_t)2;
            seq_t high = bound;

            while (low < high) {
                seq_t mid = (low + high) / 2;
                seq_t m2  = mid * mid;
                /* Use division to avoid overflow: r_num > r_den / m2   ⇔   m2*r_num > r_den */
                if (r_num > r_den / m2) {
                    high = mid;
                } else {
                    low = mid + 1;
                }
            }

            seq_t m   = low;
            seq_t m2  = m * m;

            /* Subtract 1/m^2 from the current remainder */
            seq_t new_num = r_num * m2 - r_den;
            seq_t new_den = r_den * m2;
            seq_t g = gcd(new_num, new_den);
            r_num = new_num / g;
            r_den = new_den / g;

            /* Store the denominator a(i) */
            denoms[i] = r_den;
        }
        size = n + 1;
    }

    return denoms[n];
}
