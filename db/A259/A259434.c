/*
 * A259434  Irregular table read by rows: suppose n has d decimal digits; then
 *          T(n,j) for 1 <= j <= d is the number obtained by discarding the
 *          d-j most significant digits of n.
 * https://oeis.org/A259434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) is the n‑th entry in the irregular table formed by listing,
 *            for each integer m ≥ 1, all of its decimal suffixes from the
 *            whole number down to the last digit. The j‑th suffix of m (1 ≤ j
 *            ≤ d, d = digit count of m) is T(m,j) = m mod 10^(d‑j+1). a(n) is
 *            obtained by concatenating these rows one...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  The caller may never
       pass 0, but we handle it defensively. */
    if (n == 0)
        return 0;

    idx_t cumulative = 0;

    for (idx_t m = 1; ; ++m)
    {
        idx_t d = digits(m);          /* number of decimal digits of m */
        if (cumulative + d >= n)      /* m contains the desired index */
        {
            idx_t j = n - cumulative; /* j = 1 … d */
            idx_t exp = d - j + 1;    /* power for the modulus */
            seq_t mod = pow10(exp);   /* 10^exp as seq_t */
            return (seq_t)m % mod;
        }
        cumulative += d;
    }
}
