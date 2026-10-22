/*
 * A385820  Number of equivalence classes of finitely-supported integer
 *          functions on Z^2 modulo moves that add + or -1 to every cell whose
 *          coordinates form an arithmetic progression of length n.
 * https://oeis.org/A385820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (n/gcd(n,k))^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/big_pow.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1; /** offset starts at 1; value for 0 is not used */

    seq_t result = (seq_t)1;
    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        seq_t g    = gcd((seq_t)n, (seq_t)k);   /* gcd(n,k) */
        seq_t base = (seq_t)n / g;              /* n / gcd(n,k)   */
        seq_t term = big_pow(base, (seq_t)k);   /* base^k          */
        result *= term;
    }
    return result;
}
