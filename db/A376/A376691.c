/*
 * A376691  The number of solutions to x + y == x^2 + y^2 == x^3 + y^3 (mod n)
 *          with 0 <= x <= y < n.
 * https://oeis.org/A376691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of solutions to x + y == x^2 + y^2 == x^3 + y^3 (mod n)
 *            with 0 <= x <= y < n
 * Range:     u8 n <= 11, u16 n <= 54, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t count_solutions_for_sum(idx_t n, idx_t s)
{
    seq_t cnt = 0;
    seq_t mod = (seq_t)n;
    for (idx_t x = 0; x < n; ++x)
    {
        if (s < x)          /* y would be negative */
            continue;
        idx_t y = s - x;    /* compute y */
        if (y < x || y >= n)   /* ensure x <= y and y < n */
            continue;

        seq_t sum_mod = ((seq_t)x + y) % mod;
        if (((seq_t)x * x + (seq_t)y * y) % mod != sum_mod)
            continue;
        if (((seq_t)x * x * x + (seq_t)y * y * y) % mod != sum_mod)
            continue;
        cnt++;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t mod = (seq_t)n;
    for (idx_t t = 0; t < n; ++t)
    {
        idx_t t1 = t;
        idx_t t2 = (t + n - 1) % n;   /* (t-1) mod n */
        idx_t t3 = (t + n - 2) % n;   /* (t-2) mod n */

        seq_t prod = (seq_t)t1 * (seq_t)t2 * (seq_t)t3;
        if (prod % mod != 0)
            continue; /* not a root of t(t-1)(t-2) mod n */

        /* First candidate sum s = t */
        result += count_solutions_for_sum(n, t);

        /* Second candidate sum s = t + n, if within the allowed range */
        if (n > 1 && t <= n - 2)
            result += count_solutions_for_sum(n, t + n);
    }
    return result;
}
