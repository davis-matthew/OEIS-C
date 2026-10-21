/*
 * A255231  The number of factorizations n = Product_i b_i^e_i, where all
 *          bases b_i are distinct, and all exponents e_i are distinct >=1.
 * https://oeis.org/A255231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of factorizations n = Π b_i^e_i, where all bases
 *            b_i are distinct, all exponents e_i are distinct ≥1
 * Range:     u8 n <= 419, u16 n <= 419, u32 n <= 419, u64 n <= 419,
 *            u128 n <= 419, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute r^e, returning 0 if the result would overflow the 64‑bit limit
   or exceed the supplied upper bound 'limit'. */
static uint64_t pow_limit(uint64_t r, int e, uint64_t limit)
{
    uint64_t res = 1;
    for (int i = 0; i < e; ++i)
    {
        if (res > limit / r) return 0;   /* overflow or >limit */
        res *= r;
    }
    return res;
}

/* Recursive enumeration of factorizations.
   m       : remaining part of n to factorise
   last_e  : the exponent of the previous factor (enforce strictly decreasing)
   used    : bases chosen so far
   depth   : number of bases already selected                       */
static seq_t dfs_uint64(uint64_t m, int last_e, uint64_t *used, int depth)
{
    if (m == 1) return (seq_t)1;           /* completed a valid factorisation */

    seq_t total = 0;

    /* largest possible exponent for current m (floor of log₂(m)) */
    int max_e = 0;
    uint64_t tmp = m;
    while (tmp > 1) { tmp >>= 1; ++max_e; }

    /* We can only use exponents strictly smaller than last_e. */
    int start_e = last_e - 1;
    if (start_e > max_e) start_e = max_e;

    for (int e = start_e; e >= 1; --e)
    {
        /* iterate over base candidates r ≥ 2 such that r^e divides m   */
        for (uint64_t r = 2; ; ++r)
        {
            uint64_t w = pow_limit(r, e, m);
            if (w == 0 || w > m) break;        /* no larger bases fit */

            if (m % w != 0) continue;          /* r^e does not divide m */

            /* ensure bases are distinct */
            bool dup = false;
            for (int i = 0; i < depth; ++i)
            {
                if (used[i] == r) { dup = true; break; }
            }
            if (dup) continue;

            used[depth] = r;
            total += dfs_uint64(m / w, e, used, depth + 1);
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for offset 1 */

    /* use 64‑bit arithmetic for the factorisation search */
    uint64_t m = (uint64_t)n;
    uint64_t used[32];                     /* depth will never exceed 32 for n ≤ 10000 */
    return dfs_uint64(m, 64, used, 0);
}
