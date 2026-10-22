/*
 * A386697  Values of v in the quartets (4, u, v, w) of type 2; i.e., values
 *          of v for solutions to 4(4 + u) = v(v - w), in distinct positive
 *          integers, with v > 1, sorted by nondecreasing values of u; see
 *          A386288.
 * https://oeis.org/A386697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v from solutions of 4*(4+u) = v(v‑w) in distinct positive
 *            integers 4, u, v, w (with v>1), sorted by increasing u (u ≠ 4)
 *            and, for each u, by increasing v. The index n starts at 1
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Comparator for qsort on idx_t values */
static int cmp_idx(const void *a, const void *b)
{
    const idx_t *x = (const idx_t *)a;
    const idx_t *y = (const idx_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for 0, safe fallback */

    idx_t count = 0;
    for (idx_t u = 1;; ++u)                /* iterate over all positive u ≠ 4 */
    {
        if (u == 4) continue;              /* u must be distinct from 4 */

        idx_t S = 4ULL * (4ULL + u);       /* RHS of the equation */

        /* Collect divisors of S that are candidates for v */
        static const size_t MAX_DIV = 2048;
        idx_t divs[MAX_DIV];
        size_t divc = 0;

        for (idx_t d = 1; d <= S / d; ++d) /* d iterates up to sqrt(S) safely */
        {
            if (S % d != 0) continue;

            idx_t d1 = d;
            if (d1 > 1 && d1 != 4 && d1 != u && divc < MAX_DIV)
                divs[divc++] = d1;

            idx_t d2 = S / d;
            if (d2 != d1 && d2 > 1 && d2 != 4 && d2 != u && divc < MAX_DIV)
                divs[divc++] = d2;
        }

        if (divc > 1)                      /* sort to obtain v in increasing order */
            qsort(divs, divc, sizeof(idx_t), cmp_idx);

        for (size_t i = 0; i < divc; ++i)
        {
            idx_t v = divs[i];
            if (v <= S / v) continue;          /* w = v - S/v must be positive */

            idx_t w = v - (S / v);
            if (w == 0 || w == 4 || w == u) continue;   /* distinctness check */

            /* 4, u, v, w are all distinct positive integers, v>1 satisfied */
            ++count;
            if (count == n) return (seq_t)v;
        }
    }
}
