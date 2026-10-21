/*
 * A288836  a(n) = (1/3!)*3^(n+1)*(n+5)*(n+1)*(n).
 * https://oeis.org/A288836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6) * 3^(n+1) * n * (n+1) * (n+5)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 31, u128 n <= 69,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* compute 3^(n+1) first */
    seq_t val = pow3(n + 1);

    /* cancel factors 2 and 3 from the product n*(n+1)*(n+5)
       before multiplying to avoid overflow on 64‑bit builds   */
    uint64_t f1 = n;          /* n   */
    uint64_t f2 = n + 1;      /* n+1 */
    uint64_t f3 = n + 5;      /* n+5 */

    /* helper inline to cancel a divisor < 6 from a factor */
    #define CANCEL(factor, div)          \
        do {                             \
            if ((div) > 1 && (factor) % (div) == 0) {   \
                (factor) /= (div);               \
                (div) = 1;                       \
            }                                          \
        } while (0)

    uint64_t div2 = 2;   /* divisor for factor 2 */
    uint64_t div3 = 3;   /* divisor for factor 3 */

    /* cancel the factor 2 */
    CANCEL(f1, div2);
    CANCEL(f2, div2);
    CANCEL(f3, div2);

    /* cancel the factor 3 */
    CANCEL(f1, div3);
    CANCEL(f2, div3);
    CANCEL(f3, div3);

    /* after this div2 and div3 should be 1 */
    (void)div2;
    (void)div3;

    /* multiply the remaining factors (they are small enough) */
    val *= (seq_t)f1;
    val *= (seq_t)f2;
    val *= (seq_t)f3;

    return val;
}
