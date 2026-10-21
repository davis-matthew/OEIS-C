/*
 * A253714  Indices of hexagonal numbers (A000384) which are also centered
 *          heptagonal numbers (A069099).
 * https://oeis.org/A253714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+64514*a(n-2)-64514*a(n-3)-a(n-4)+a(n-5); Initial
 *            values: a(1)=1, a(2)=2857, a(3)=45529, a(4)=184300369,
 *            a(5)=2937241777
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t init[5] = {
        1,
        2857,
        45529,
        184300369,
        2937241777
    };

    if (n <= 5) {
        return init[n - 1];
    }

    seq_t t1 = init[0];          /* a(n-5)  */
    seq_t t2 = init[1];          /* a(n-4)  */
    seq_t t3 = init[2];          /* a(n-3)  */
    seq_t t4 = init[3];          /* a(n-2)  */
    seq_t t5 = init[4];          /* a(n-1)  */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t t6 = t5 + (seq_t)64514 * (t4 - t3) - t2 + t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = t5;
        t5 = t6;
    }

    return t5;
}
