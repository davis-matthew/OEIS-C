/*
 * A357192  a(n) = n! * Sum_{k=0..floor(n/3)} k^n/k!.
 * https://oeis.org/A357192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} k^n / k!
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate an array for factorials 0..n */
    seq_t *fac = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!fac) return (seq_t)0;   /* allocation failure – unlikely in tests */

    fac[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fac[i] = fac[i-1] * (seq_t)i;   /* fac[i] = i! */
    }

    seq_t fact_n = fac[n];
    seq_t sum = (seq_t)0;
    idx_t m = n / 3;   /* upper bound for k */

    for (idx_t k = 1; k <= m; ++k) {
        seq_t k_pow_n = big_pow((seq_t)k, (seq_t)n);   /* k^n  */
        seq_t ratio   = fact_n / fac[k];               /* n! / k!  (exact) */
        sum += ratio * k_pow_n;
    }

    free(fac);
    return sum;   /* a(n) */
}
