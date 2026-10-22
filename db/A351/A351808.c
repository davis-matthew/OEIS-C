/*
 * A351808  a(n) is the quotient obtained when pod(m) divides pod(m^2), with
 *          pod = product of digits = A007954 and m = A351807(n).
 * https://oeis.org/A351808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = pod(m(n)^2) / pod(m(n)) * where m(n) is the nth positive
 *            integer k such that * pod(k) ≠ 0 and pod(k^2) is divisible by
 *            pod(k). * Here pod(x) is the product of the decimal digits of x
 *            (A007954)
 * Range:     u8 n <= 10, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* not used, but safe */

    static seq_t *ratios = NULL;    /* stored sequence values */
    static size_t cap = 0;          /* capacity of the array      */
    static size_t sz  = 0;          /* number of computed terms   */
    static idx_t cand = 1;          /* candidate integer to test */

    while (sz < n)
    {
        seq_t pod_cur = digit_product((seq_t)cand);
        if (pod_cur != (seq_t)0)          /* skip numbers containing 0 */
        {
            seq_t sq   = (seq_t)cand * cand;
            seq_t pod_sq = digit_product(sq);
            if ((pod_sq % pod_cur) == (seq_t)0)
            {
                seq_t ratio = pod_sq / pod_cur;

                if (sz == cap)            /* grow array when needed   */
                {
                    size_t new_cap = (cap == 0) ? 16 : cap * 2;
                    seq_t *tmp = (seq_t*)realloc(ratios, new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();           /* allocation failed       */
                    ratios = tmp;
                    cap = new_cap;
                }
                ratios[sz++] = ratio;
            }
        }
        ++cand;
    }

    return ratios[n - 1];
}
