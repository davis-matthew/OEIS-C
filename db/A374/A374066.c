/*
 * A374066  a(n) is the number of terms in the trajectory when the map x ->
 *          A067240(x) is iterated, starting from x = n until x = 0.
 * https://oeis.org/A374066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(A067240(n)) for n >= 1, with a(0) = 1 (terms
 *            counted from n down to 0); A067240(n) = Sum_{p^e || n}
 *            (p-1)*p^(e-1), A067240(1) = 0
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 89 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static idx_t A374066_f(idx_t n)
{
    if (n <= 1) return 0;
    idx_t result = 0;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t term = p - 1;
            m /= p;
            while (m % p == 0) {
                m /= p;
                term *= p;
            }
            result += term;
        }
    }
    if (m > 1) result += m - 1;
    return result;
}

seq_t a(idx_t n)
{
    seq_t count = 1; /* counts the term 0 */
    idx_t x = n;
    while (x != 0) {
        ++count;
        x = A374066_f(x);
    }
    return count;
}
