/*
 * A307070  a(n) is the number of decimal places before the decimal expansion
 *          of 1/n terminates, or the period of the recurring portion of 1/n
 *          if it is recurring.
 * https://oeis.org/A307070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A307070:
   Let n = 2^a * 5^b * m with gcd(m,10)=1.
   If m = 1   -> a(n) = max(a,b)   (terminating decimal)
   Else        a(n) = multiplicative order of 10 modulo m   (recurring period) */

#include <stdint.h>
#include "../utils/mod_mul.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;           /* 1/1 = 1.0 has 0 decimal places */

    idx_t tmp = n;
    idx_t cnt2 = 0, cnt5 = 0;

    /* factor out powers of 2 */
    while ((tmp & 1) == 0) { tmp >>= 1; cnt2++; }

    /* factor out powers of 5 */
    while ((tmp % 5) == 0) { tmp /= 5; cnt5++; }

    /* terminating decimal if nothing left of 2 or 5 */
    if (tmp == 1) {
        return (seq_t)(cnt2 > cnt5 ? cnt2 : cnt5);
    }

    /* Non‑terminating: m = tmp coprime to 10.
       Find the order of 10 modulo m. */
    seq_t period = 1;
    idx_t mod = (idx_t)(10 % tmp);

    while (mod != 1) {
        mod = (idx_t)mod_mul((uint64_t)mod, (uint64_t)10, (uint64_t)tmp);
        period++;
    }

    return period;
}
