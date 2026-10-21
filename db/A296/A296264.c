/*
 * A296264  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)*b(n-1), where a(0) = 2, a(1) = 4, b(0) = 1, b(1) = 3, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A296264
    The increasing complementary sequences a(n) and b(n) satisfy
    a(n) = a(n-1) + a(n-2) + b(n-2)*b(n-1)   (n ≥ 2)
    with a(0)=2, a(1)=4,  b(0)=1, b(1)=3.
    The sequence a(n) is returned by this function.  It is computed
    together with its complementary sequence b(n), using a binary
    search for the nth missing integer.
*/

#include <stdlib.h>
#include <stddef.h>

/*  Ensure that *arr has capacity at least need.  */
static void ensure_cap(size_t need, seq_t **arr, size_t *cap)
{
    if (need <= *cap) return;
    size_t newcap = (*cap == 0) ? need * 2 : (*cap << 1);
    if (newcap < need) newcap = need;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

seq_t a(idx_t n)
{
    /*  Static storage for the sequences.  */
    static seq_t *aArr = NULL;
    static size_t aCap = 0, aSize = 0;
    static seq_t *bArr = NULL;
    static size_t bCap = 0, bSize = 0;

    /*  Initialise first two terms once.  */
    if (aSize == 0) {
        ensure_cap(4, &aArr, &aCap);
        aArr[0] = (seq_t)2;
        aArr[1] = (seq_t)4;
        aSize = 2;

        ensure_cap(4, &bArr, &bCap);
        bArr[0] = (seq_t)1;
        bArr[1] = (seq_t)3;
        bSize = 2;
    }

    /*  Extend the sequences up to n.  */
    for (idx_t i = aSize; i <= n; ++i) {
        /*  a(i) = a(i-1) + a(i-2) + b(i-2)*b(i-1)  */
        seq_t ai = aArr[i - 1] + aArr[i - 2]
                 + bArr[i - 2] * bArr[i - 1];

        ensure_cap(aSize + 1, &aArr, &aCap);
        aArr[aSize] = ai;
        ++aSize;

        /*  Compute b(i) as the (i+1)-th missing integer.  */
        size_t missing_needed = (size_t)(i + 1);   /*  i+1  */
        seq_t low  = (seq_t)1;
        seq_t high = ai + (seq_t)i + 1;             /*  safe upper bound  */

        while (low < high) {
            seq_t mid = low + (high - low) / 2;

            /*  Count number of a-values ≤ mid (upper_bound).  */
            size_t left = 0, right = aSize;
            while (left < right) {
                size_t m = (left + right) / 2;
                if (aArr[m] <= mid)
                    left = m + 1;
                else
                    right = m;
            }
            size_t cnt = left;  /*  a-values ≤ mid  */
            seq_t missing = mid - (seq_t)cnt;

            if (missing >= (seq_t)missing_needed)
                high = mid;
            else
                low = mid + 1;
        }

        ensure_cap(bSize + 1, &bArr, &bCap);
        bArr[bSize] = low;
        ++bSize;
    }

    return aArr[n];
}
