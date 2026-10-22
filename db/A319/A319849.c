/*
 * A319849  Irregular triangle read by rows in which row n lists the even
 *          divisors of n in increasing order together with the odd divisors
 *          of n in increasing order.
 * https://oeis.org/A319849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the n-th element of the irregular triangle where each row
 *            n lists the even divisors of n first (in increasing order)
 *            followed by the odd divisors (in increasing order). This
 *            function searches rows until it reaches the desired index and
 *            then returns the appropriate divisor
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Comparator for qsort on unsigned long long */
static int cmp_ull(const void *a, const void *b)
{
    unsigned long long x = *(const unsigned long long *)a;
    unsigned long long y = *(const unsigned long long *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety guard – sequence starts at 1 */

    idx_t sum = 0;                  /* cumulative number of elements processed */
    idx_t r   = 1;                  /* current row number */

    while (1)
    {
        /* Collect divisors of r, separating even and odd.  Use a fixed buffer
           large enough for the expected number of divisors (more than enough
           for n up to 10 000). */
        unsigned long long evenDivs[512], oddDivs[512];
        size_t evenCnt = 0, oddCnt = 0;

        for (idx_t d = 1; (idx_t)(d * d) <= r; ++d)
        {
            if (r % d == 0)
            {
                unsigned long long d1 = d;
                unsigned long long d2 = r / d;

                if ((d1 & 1ULL) == 0ULL)
                    evenDivs[evenCnt++] = d1;
                else
                    oddDivs[oddCnt++] = d1;

                if (d1 != d2)
                {
                    if ((d2 & 1ULL) == 0ULL)
                        evenDivs[evenCnt++] = d2;
                    else
                        oddDivs[oddCnt++] = d2;
                }
            }
        }

        size_t totalCnt = evenCnt + oddCnt;

        if (sum + totalCnt >= n)          /* the target element is in row r */
        {
            idx_t offset = n - sum;        /* 1‑based position in this row */

            if (offset <= evenCnt)
            {
                qsort(evenDivs, evenCnt, sizeof(unsigned long long), cmp_ull);
                return (seq_t)evenDivs[offset - 1];
            }
            else
            {
                qsort(oddDivs, oddCnt, sizeof(unsigned long long), cmp_ull);
                return (seq_t)oddDivs[offset - evenCnt - 1];
            }
        }

        sum += totalCnt;
        ++r;
    }
}
