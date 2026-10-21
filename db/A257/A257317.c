/*
 * A257317  Number of unordered ways to write n as the sum of two distinct
 *          elements of the set {floor(x/3): 3*x-1 and 3*x+1 are twin prime}
 *          one of which is even.
 * https://oeis.org/A257317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs {x,y} with x ≠ y, x,y ∈ S =
 *            {⌊t/3⌋ : 3t−1 and 3t+1 are twin primes}, x+y=n, and at least one
 *            of x or y is even. Multiple unordered ways are counted. The set
 *            S is grown as needed for the largest n requested
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static idx_t max_n = 0;          /* largest n for which S has been built */
static bool *seen = NULL;       /* seen[v] == true if v ∈ S, 0 ≤ v ≤ max_n */

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1, return 0 */

    /* Rebuild seen array if a larger n is requested */
    if (n > max_n) {
        /* Allocate a fresh bool array of length n+1 */
        bool *new_seen = (bool *)calloc((size_t)(n + 1), sizeof(bool));
        if (!new_seen) return 0;        /* allocation failure yields 0 */

        /* Free the old array if it existed */
        if (seen) free(seen);
        seen = new_seen;
        max_n = n;

        /* Generate all values floor(t/3) for t such that 3t−1 and 3t+1 are
           twin primes, and mark them in the array.  t ranges up to 3n+2
           because floor(t/3) ≤ n ⇔ t ≤ 3n+2. */
        for (idx_t t = 1; t <= 3 * n + 2; ++t) {
            idx_t p1 = 3 * t - 1;
            idx_t p2 = 3 * t + 1;
            if (!is_prime(p1) || !is_prime(p2))
                continue;
            idx_t v = t / 3;               /* floor division */
            if (v <= n)
                seen[v] = true;
        }
    }

    /* Count unordered distinct pairs (a,b) with a+b=n, a<b.  At least one
       of a or b must be even. */
    seq_t count = 0;
    for (idx_t a_val = 0; a_val <= n; ++a_val) {
        if (!seen[a_val])
            continue;
        idx_t b_val = n - a_val;
        if (b_val <= a_val || b_val > n || !seen[b_val])
            continue;
        if (((a_val & 1) == 0) || ((b_val & 1) == 0))
            ++count;
    }
    return count;
}
