/*
 * A323665  a(n) is the number of vertices in the binary tree the root of
 *          which is assigned the value n and built recursively by the rule:
 *          write node's value as (2^c)*(2k+1); if c>0, create a left child
 *          with value c; if k>0, create a right child with value k.
 * https://oeis.org/A323665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recursively count the vertices of the tree defined by
    a(1) = 1
    a(n) = 1
           + (f > 0 ? a(f) : 0)      // left child: value of the exponent of 2 in n
           + (k > 0 ? a(k) : 0);     // right child: k = (odd_part-1)/2
*/
/*  Args:
    n -- positive integer index (1-based).  The function returns the number of
         vertices in the binary tree whose root is assigned the value n.
*/
#include <stdint.h>

static seq_t rec(idx_t n)
{
    if (n <= 1)    /* n==1 gives only the root */
        return 1;

    /* decompose n = 2^c * (2k+1), with c>=0, k>=0   */
    idx_t temp = n;
    idx_t c = 0;
    while ((temp & 1ULL) == 0) { /* while even */
        c++;
        temp >>= 1;
    }
    /* temp is now the odd part 2k+1 */
    seq_t res = 1;                 /* count the root */
    if (c > 0)
        res += rec(c);            /* left child */
    if (temp > 1) {
        idx_t k = (temp - 1) >> 1;
        if (k > 0)
            res += rec(k);        /* right child */
    }
    return res;
}

seq_t a(idx_t n)
{
    return rec(n);
}
