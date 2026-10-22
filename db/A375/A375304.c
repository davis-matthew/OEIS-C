/*
 * A375304  Triangle read by rows, T(n,k) = Sum_{i=0..n}
 *          (-1)^i*(k!)^i*(n*k-i*k)!, 0<=k<=n.
 * https://oeis.org/A375304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{i=0..n} (-1)^i * (k!)^i * (n*k - i*k)!
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 13, u64 n <= 19, u128 n <= 26,
 *            big any n
 * Helpers:   ensure_fact, factorial_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows, a(0) is the first term. */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/ensure_fact.h"
#include "../utils/factorial_at.h"

seq_t a(idx_t n)
{
    /* Find row r and column k such that n lies in row r */
    double d = sqrt((long double)8.0 * (long double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);
    /* adjust in case of rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    idx_t prefix = r * (r + 1) / 2;
    idx_t k = n - prefix;          /* 0 <= k <= r */

    /* Special case k == 0: T(r,0) = alternating sum of ones */
    if (k == 0) {
        return (r % 2 == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* Ensure factorials up to needed size */
    idx_t max_fact = k * r;
    ensure_fact(max_fact);
    idx_t max_k_fact = k;
    ensure_fact(max_k_fact);

    seq_t fact_k = factorial_at(k);

    seq_t sum = 0;
    seq_t pow_factor = 1;  /* (k!)^t, starts with t=0 */

    for (idx_t t = 0; t <= r; ++t) {
        /* factorial of k*(r - t) */
        idx_t idx_fact = k * (r - t);
        seq_t term = pow_factor * factorial_at(idx_fact);

        if (t & 1)
            sum -= term;
        else
            sum += term;

        if (t < r)
            pow_factor *= fact_k;  /* advance to next power */
    }
    return sum;
}
