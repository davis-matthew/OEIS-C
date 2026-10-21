/*
 * A244912  Sum of leading digits in representations of n in bases 2,3,...,n.
 * https://oeis.org/A244912
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k=2..n} ⌊ n / k^⌊log_k n⌋ ⌋ That is, for each base k,
 *            take the highest power of k not exceeding n, divide n by that
 *            power, and sum the leading digits
 * Range:     u8 n <= 117, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* values below the offset are irrelevant */

    seq_t nn   = (seq_t)n; /* cast index to seq_t for arithmetic */
    seq_t sum  = 0;        /* running total */

    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t kseq = (seq_t)k;
        seq_t kpow = 1;

        /* find the greatest power kpow = k^m <= n, using safe multiplication */
        while (kpow <= nn / kseq)
            kpow *= kseq;

        sum += nn / kpow;  /* leading digit in base k */
    }

    return sum;
}
