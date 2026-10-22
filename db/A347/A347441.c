/*
 * A347441  Number of odd-length factorizations of n with integer alternating
 *          product.
 * https://oeis.org/A347441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9935, u16 n <= 9935, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Count odd‑length weakly increasing factorizations of n
   for which the alternating product (product of odd–indexed factors
   divided by product of even–indexed factors) is an integer.  For
   a factorization y1…yk, this is equivalent to requiring that
   (∏ y_even)^2 divides n.  The function performs a recursive
   enumeration of all such factorizations. */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

static void rec(uint64_t n, uint64_t start, uint64_t *cur,
                size_t len, uint64_t orig, seq_t *cnt)
{
    /* Include the remaining factor n as the last element */
    if (n >= start) {
        cur[len] = n;
        len++;
        if (len & 1) {                      /* odd length */
            uint64_t eprod = 1;
            for (size_t i = 1; i < len; i += 2)   /* y2, y4, …  */
                eprod *= cur[i];
            if (eprod != 0 && orig % (eprod * eprod) == 0)
                (*cnt) += 1;
        }
        len--;                               /* backtrack */
    }

    uint64_t limit = (uint64_t)sqrt((double)n);
    for (uint64_t d = start; d <= limit; ++d) {
        if (n % d == 0) {
            cur[len] = d;
            rec(n / d, d, cur, len + 1, orig, cnt);
        }
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    uint64_t N = (uint64_t)n;
    uint64_t cur[64];
    seq_t count = 0;
    rec(N, 2, cur, 0, N, &count);
    return count;
}
