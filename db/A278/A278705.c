/*
 * A278705  Number of length-4 cycles in all simple labeled graphs on n nodes.
 * https://oeis.org/A278705
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,4)*3*2^(binomial(n,2)-4)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is not defined; return 0. */
    if (n < 4) {
        return (seq_t)0;
    }

    /* Compute the binomial coefficient C(n,4), which gives the number
       of 4‑vertex subsets. */
    seq_t cn4 = binomial(n, 4);

    /* Number of edges in a complete graph on n nodes. */
    idx_t total_edges = (n * (n - 1)) / 2;

    /* Exponent for 2^(C(n,2)-4). */
    idx_t exp = total_edges - 4;

    /* 2^(C(n,2)-4). */
    seq_t pow2_val = pow2(exp);

    /* Final result: 3 * C(n,4) * 2^(C(n,2)-4). */
    seq_t res = cn4 * (seq_t)3;
    res = res * pow2_val;

    return res;
}
