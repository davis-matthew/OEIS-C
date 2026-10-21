/*
 * A281914  Number of ordered integer pairs (b,c), with -n<=b<=n, -n<=c<=n,
 *          such that both roots of 3x^2+bx+c = 0 are rational and b and are
 *          not both multiples of 3.
 * https://oeis.org/A281914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #ordered integer pairs (b,c), -n ≤ b,c ≤ n, * such that
 *            the discriminant b^2-12c is a perfect square * and b and c are
 *            not both multiples of 3
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    int64_t N = (int64_t)n;
    seq_t total = 0;
    // Loop over c from -N to N
    for (int64_t c = -N; c <= N; ++c)
    {
        if (c == 0)
        {
            // All b in [-N,N] are valid except those where b is multiple of 3
            seq_t total_b   = (seq_t)(2 * N + 1);
            seq_t mult3_b   = 2 * (seq_t)(N / 3) + 1;  // count of b divisible by 3
            total += total_b - mult3_b;
            continue;
        }

        int64_t t = 12 * c;                       // discriminant relation
        int64_t abs_t = t < 0 ? -t : t;
        // Enumerate positive divisors d of abs_t up to sqrt(abs_t)
        int64_t root = (int64_t)sqrt((double)abs_t);
        while ((root + 1) * (root + 1) <= abs_t) root++;
        while (root * root > abs_t) root--;

        for (int64_t d = 1; d <= root; ++d)
        {
            if (abs_t % d != 0)
                continue;
            int64_t q = abs_t / d;  // positive quotient

            // Pair 1: (p, q) = (d, t/d)
            int64_t p1 = d;
            int64_t q1 = t / d;          // exact division
            if (((p1 & 1) == (q1 & 1)))
            {
                int64_t b1 = (p1 + q1) / 2;
                if (b1 >= -N && b1 <= N &&
                    !( (c % 3 == 0) && (b1 % 3 == 0) ))
                    total++;
            }

            // Pair 2: (p, q) = (-d, -t/d)
            // Avoid double counting when t < 0 and d*d == abs_t
            if (! (t < 0 && d == q))
            {
                int64_t p2 = -d;
                int64_t q2 = -t / d;          // - (t/d) exactly
                if (((p2 & 1) == (q2 & 1)))
                {
                    int64_t b2 = (p2 + q2) / 2;
                    if (b2 >= -N && b2 <= N &&
                        !( (c % 3 == 0) && (b2 % 3 == 0) ))
                        total++;
                }
            }
        }
    }
    return total;
}
