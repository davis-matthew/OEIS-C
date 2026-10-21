/*
 * A297146  Numbers having an up-first zigzag pattern in base 10; see
 *          Comments.
 * https://oeis.org/A297146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n‑th integer ≥12 whose decimal digits have no equal
   consecutive digits and whose first two digits are increasing (up‑first
   zigzag).  The sequence is enumerated in increasing order.   */
#include <stdbool.h>

static bool up_first_zigzag(seq_t x)
{
    /* need at least two digits */
    if (x < (seq_t)10) return false;

    /* maximum digits expected for reasonably small n; 512 ≫ the required
       length for n up to about 10⁶ in base 10.            */
    int d[512];
    int len = 0;
    seq_t t = x;

    /* collect digits in little‑endian order */
    while (t > (seq_t)0) {
        d[len++] = (int)(t % (seq_t)10);
        t /= (seq_t)10;
    }

    /* most significant digit is d[len-1] */
    if (d[len-1] >= d[len-2]) return false;   /* first comparison must be up */

    /* check all adjacent digits are distinct */
    for (int i = len-1; i > 0; --i) {
        if (d[i] == d[i-1]) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour below offset is irrelevant */

    idx_t count = 0;
    seq_t cur = (seq_t)12;         /* first possible candidate */

    for (;;) {
        if (up_first_zigzag(cur)) {
            ++count;
            if (count == n) return cur;
        }
        ++cur;
    }
}
