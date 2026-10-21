/*
 * A247615  a(n) = Sum_{k=0..10} binomial(20,k)*binomial(n,k).
 * https://oeis.org/A247615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..10} binomial(20,k)*binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 15, u64 n <= 117, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* pre‑computed binomial(20,k) for k = 0..10 */
    static const seq_t C20[11] = {
        (seq_t)1,     (seq_t)20,    (seq_t)190,    (seq_t)1140,
        (seq_t)4845,  (seq_t)15504, (seq_t)38760,  (seq_t)77520,
        (seq_t)125970,(seq_t)167960,(seq_t)184756
    };

    seq_t res = 0;
    for (idx_t k = 0; k <= 10; ++k) {
        if (n < k) break;            /* binomial(n,k)=0 for n<k */
        seq_t cnk = binomial(n, k);
        res += C20[k] * cnk;
    }
    return res;
}
