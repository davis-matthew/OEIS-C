/*
 * A370292  Row sums of A370222.
 * https://oeis.org/A370292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of inversions of the n-th stack-realizable
 *            (312-avoiding) permutation, taken in lexicographic order within
 *            each length m = 1, 2, ... (a row sum of A370222). Computed by
 *            unranking the stack push/pop word in lex order; each pop adds
 *            the stack size left after it to the inversion count
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9555 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A370292_MAXM 35
#define A370292_MAXS 72

static uint64_t A370292_N[A370292_MAXS][A370292_MAXM + 1];
static int A370292_ready = 0;

/* N[s][u] = number of ways to finish when s values are on the stack
   and u values have not been pushed yet */
static void A370292_setup(void)
{
    if (A370292_ready) return;
    for (int u = 0; u <= A370292_MAXM; ++u)
    {
        for (int s = 0; s < A370292_MAXS; ++s)
        {
            if (u == 0) { A370292_N[s][0] = 1; continue; }
            uint64_t c = (s > 0) ? A370292_N[s - 1][u] : 0;
            for (int j = 0; j < u; ++j)
            {
                if (s + j < A370292_MAXS) c += A370292_N[s + j][u - j - 1];
            }
            A370292_N[s][u] = c;
        }
    }
    A370292_ready = 1;
}

seq_t a(idx_t n)
{
    A370292_setup();
    uint64_t r = (uint64_t)n - 1;
    int m = 1;
    while (m < A370292_MAXM && r >= A370292_N[0][m])
    {
        r -= A370292_N[0][m];
        ++m;
    }
    int s = 0, u = m;
    uint64_t inv = 0;
    while (s > 0 || u > 0)
    {
        if (s > 0)
        {
            uint64_t c = A370292_N[s - 1][u];
            if (r < c)
            {
                --s;
                inv += (uint64_t)s;
                continue;
            }
            r -= c;
        }
        for (int j = 0; j < u; ++j)
        {
            uint64_t c = A370292_N[s + j][u - j - 1];
            if (r < c)
            {
                s = s + j;
                inv += (uint64_t)s;
                u = u - j - 1;
                break;
            }
            r -= c;
        }
    }
    return (seq_t)inv;
}
