/*
 * A276348  a(n) = the smallest number k such that k*n is a number with a
 *          string of 1's followed by a string of 0's.
 * https://oeis.org/A276348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A052983(n)/n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   pow10, pow2, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = smallest k such that k·n is a number made of a string of 1's followed by a string of 0's. */

#include "../utils/pow10.h"
#include "../utils/pow2.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */

    /* factor out powers of 2 and 5 */
    idx_t temp = n;
    idx_t a2 = 0;              /* exponent of 2 */
    idx_t b5 = 0;              /* exponent of 5 */

    while ((temp & 1ULL) == 0) { temp >>= 1; ++a2; }
    while (temp % 5ULL == 0) { temp /= 5; ++b5; }

    idx_t m = temp;            /* the remaining factor, coprime with 10 */

    /* compute d = max(a2, b5) (or 1 if both are zero) */
    idx_t d = (a2 == 0 && b5 == 0) ? 1 : (a2 > b5 ? a2 : b5);

    /* find minimal q such that 9*m | 10^q - 1 */
    seq_t mod = (seq_t)9 * (seq_t)m;
    seq_t pow = (seq_t)10 % mod;
    idx_t q = 1;
    while (pow != 1) {
        pow = (pow * (seq_t)10) % mod;
        ++q;
    }

    /* compute repunit (10^q - 1) / 9 and divide by m */
    seq_t repunit = (pow10(q) - (seq_t)1) / 9;
    seq_t k = repunit / (seq_t)m;     /* this is the part (10^q - 1)/(9*m) */

    /* multiply by remaining powers of 2 and 5 */
    k *= pow2(d - a2);
    k *= power5(d - b5);

    return k;
}
