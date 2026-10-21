/*
 * A260819  Positive integer solutions [a',b',c',d'] of the equation 1/a + 1/b
 *          + 1/c + 1/d = 1, for a <= b <= c <= d, lexicographically sorted.
 * https://oeis.org/A260819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, full, fini, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   _gcd_idx, _lcm_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate all positive integer solutions of
   1/a + 1/b + 1/c + 1/d = 1 with a <= b <= c <= d.
   The solutions are stored lexicographically and the n-th term
   (a(1)=2, a(2)=3, …) is returned. */

/* Required includes for C/C++ dual compilation */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_gcd_idx.h"
#include "../utils/_lcm_idx.h"

/* Helper: Euclidean gcd for idx_t */

/* Storage for all solutions */
static seq_t _solutions[16][4] = { {0} };  /* 14 solutions known */
static idx_t _solutions_cnt = 0;            /* number of solutions found */

/* Compute all solutions once */
static void _init_solutions(void)
{
    if (_solutions_cnt != 0)            /* already computed */
        return;

    /* a ranges from 2 to 4 (larger values impossible) */
    for (idx_t a = 2; a <= 4; ++a) {
        /* b ranges from a upwards; upper bound 200 is safe */
        for (idx_t b = a; b <= 200; ++b) {
            idx_t l_ab = _lcm_idx(a, b);
            idx_t sum_ab = l_ab / a + l_ab / b;
            if (sum_ab >= l_ab)          /* 1/a + 1/b >= 1, impossible */
                continue;

            idx_t rem_ab = l_ab - sum_ab;        /* 1 - (1/a+1/b) in denominator l_ab */
            idx_t max_c = (2 * l_ab) / rem_ab;   /* from 1/a+1/b+2/c >= 1 */

            if (max_c < b)
                continue;

            for (idx_t c = b; c <= max_c; ++c) {
                idx_t l_abc = _lcm_idx(l_ab, c);
                idx_t sum_abc = l_abc / a + l_abc / b + l_abc / c;
                if (sum_abc >= l_abc)
                    continue;                 /* sum already >= 1, impossible */

                idx_t rem = l_abc - sum_abc;  /* remaining part to reach 1 */
                if (rem == 0)
                    continue;
                if (l_abc % rem != 0)
                    continue;

                /* d is integer and satisfies a <= b <= c <= d */
                seq_t d = (seq_t)(l_abc / rem);
                if (d < c)
                    continue;

                /* Store the solution */
                _solutions[_solutions_cnt][0] = (seq_t)a;
                _solutions[_solutions_cnt][1] = (seq_t)b;
                _solutions[_solutions_cnt][2] = (seq_t)c;
                _solutions[_solutions_cnt][3] = d;
                ++_solutions_cnt;
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    _init_solutions();          /* compute solutions once */

    if (n == 0)                 /* offset 1; undefined below 1 */
        return (seq_t)0;

    idx_t idx = n - 1;
    idx_t group = idx / 4;
    idx_t pos   = idx % 4;

    if (group >= _solutions_cnt)
        return (seq_t)0;        /* out of bounds – should not happen */

    return _solutions[(size_t)group][(size_t)pos];
}
