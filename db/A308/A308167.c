/*
 * A308167  Number of integer-sided triangles with perimeter n and sides a, b,
 *          and c such that a <= b <= c and a|b.
 * https://oeis.org/A308167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer-sided triangles with perimeter n, sides
 *            a,b,c such that a <= b <= c and a divides b
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm iterates over possible smallest side a, and determines
   the allowed range of the quotient k = b/a. The count is computed in
   O(n) time. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Count of triangles for perimeter n */
    seq_t count = 0;

    /* Smallest side a must satisfy a <= n/3, otherwise all sides would be too
       large to sum to n. */
    idx_t max_a = n / 3;

    for (idx_t a_side = 1; a_side <= max_a; ++a_side)
    {
        /* Boundaries for k = b/a */

        /* Minimum k from triangle inequality a + k*a > n/2   => k >= floor(n/(2a)) */
        idx_t k_min = n / (2 * a_side);
        if (k_min == 0)  /* ensure k >= 1 */
            k_min = 1;

        /* Maximum k from the requirement that c = n - (k+1)a >= 1   => (k+1)a <= n-1   */
        idx_t temp_max_c = (n - 1) / a_side;                  /* floor((n-1)/a) */
        idx_t k_max1 = (temp_max_c >= 1) ? temp_max_c - 1 : 0; /* floor((n-1)/a) - 1 */

        /* Maximum k from b <= c   => k*a <= n - (k+1)a   => (2k+1)a <= n   */
        idx_t temp_n_div_a = n / a_side;                      /* floor(n/a) */
        idx_t k_max2 = 0;
        if (temp_n_div_a >= 1)
            k_max2 = (temp_n_div_a - 1) / 2;                  /* floor((n/a - 1)/2) */

        /* Overall maximum k is the smaller of the two bounds */
        idx_t k_max = (k_max1 < k_max2) ? k_max1 : k_max2;

        if (k_max >= k_min)
            count += (seq_t)(k_max - k_min + 1);
    }

    return count;
}
