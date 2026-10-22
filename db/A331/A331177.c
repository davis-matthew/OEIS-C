/*
 * A331177  Number of values of k, 1 <= k <= n, with A319677(k) = A319677(n),
 *          where A319677(n) = n/gcd(n, uphi(n)), and uphi is unitary totient
 *          function (A047994).
 * https://oeis.org/A331177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k, 1 ≤ k ≤ n, with A319677(k) = A319677(n),
 *            where A319677(n) = n / gcd(n, Uphi(n)) and Uphi is the unitary
 *            totient (A047994). The sequence is computed by a simple O(n·√n)
 *            algorithm
 * Range:     u8 n <= 358, u16 n <= 9475, u32 n <= 9475, u64 n <= 9475,
 *            u128 n <= 9475, big any n
 * Helpers:   gcd, unitary_totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/unitary_totient.h"

/* A319677(n) = n / gcd(n, Uphi(n)) */
static seq_t a319677_value(idx_t n)
{
    seq_t uphi = unitary_totient(n);
    seq_t g = gcd((seq_t)n, uphi);
    return (seq_t)n / g;
}

/* External interface expected by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */
    seq_t target = a319677_value(n);
    seq_t count  = 1;                     /* count n itself */
    for (idx_t k = 1; k < n; ++k) {
        if (a319677_value(k) == target)
            ++count;
    }
    return count;
}
