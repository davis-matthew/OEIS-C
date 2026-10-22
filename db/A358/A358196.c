/*
 * A358196  Numbers k such that 5^k and 8^k have the same leading digit.
 * https://oeis.org/A358196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A358196: Numbers k such that 5^k and 8^k have the same
 * leading digit.  Index starts at 1 with a(1)=0.
 *
 * The algorithm iterates k=0,1,2,..., keeping a counter of how many
 * values satisfy the condition.  For each k it computes the fractional
 * part of k*log10(2) using long double arithmetic.  From this fractional
 * part it obtains the leading digit of 5^k and 8^k:
 *
 *     frac5 = {k*log10(5)} = 1 - {k*log10(2)}          (mod 1)
 *     frac8 = {k*log10(8)} = {3*k*log10(2)}            (mod 1)
 *
 * The leading digit is floor(10^frac).  A small epsilon protects
 * against rounding errors when frac is very close to an integer.
 *
 * The function returns (seq_t)k when the counter reaches n.
 * It works for any n≥1 and in any call order.
 */

#include <math.h>
#include <float.h>

seq_t a(idx_t n)
{
    static const long double LOG10_2 = 0.3010299956639811952137388947244930267682L; /* log10(2) */
    static const long double LOG10_5 = 1.0L - LOG10_2;                               /* log10(5) */
    static const long double LOG10_8 = LOG10_2 * 3.0L;                               /* log10(8) */

    idx_t  k      = 0;          /* current exponent */
    idx_t  count  = 0;          /* how many good k's found */
    const long double EPS = 1e-15L;

    while (1) {
        /* fractional part of k*log10(2) */
        long double x = fmodl((long double)k * LOG10_2, 1.0L);

        /* leading digit of 5^k */
        long double frac5 = 1.0L - x;
        if (frac5 >= 1.0L - EPS)       /* x == 0 */
            frac5 = 0.0L;
        int d5 = (int)floorl( powl(10.0L, frac5) + EPS );

        /* leading digit of 8^k */
        long double frac8 = fmodl(x * 3.0L, 1.0L);
        int d8 = (int)floorl( powl(10.0L, frac8) + EPS );

        if (d5 == d8) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
