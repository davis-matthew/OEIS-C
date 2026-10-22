/*
 * A308820  a(n) = Product_{k=1..n} ceiling(n/k)!.
 * https://oeis.org/A308820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{n} ⌈n/k⌉! = ∏_{k=1}^{n-1} (k+1)^{⌊(n-1)/k⌋}
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;   /* out‑of‑range, safety */
    seq_t result = (seq_t)1;
    idx_t n1 = n - 1;              /* n-1 fits in idx_t */

    for (idx_t k = 1; k <= n1; ++k) {
        idx_t exp = n1 / k;                   /* floor((n-1)/k)   */
        seq_t base = (seq_t)(k + 1);           /* (k+1)            */
        seq_t pow  = big_pow(base, (seq_t)exp);
        result = seq_mul(result, pow);
    }
    return result;
}
