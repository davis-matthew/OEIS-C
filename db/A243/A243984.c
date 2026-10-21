/*
 * A243984  Sum of non-twin divisors of n.
 * https://oeis.org/A243984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = sum of divisors d of n that are NOT twin divisors.
   A divisor d of n is a twin divisor if another divisor d' of n satisfies
   |d - d'| = 2.  This function enumerates all divisors of n, marks those
   that are twin, and returns the sum of the remaining divisors. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;          /* offset handling – not used by OEIS */

    /* Maximum number of divisors for n up to 10000 is well below 512. */
    seq_t divs[512];
    size_t count = 0;
    bool twin[512] = {0};

    /* Enumerate all divisors of n. */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            divs[count++] = (seq_t)i;
            if (i != n / i)           /* avoid double counting a square root */
                divs[count++] = (seq_t)(n / i);
        }
    }

    /* Mark twin divisors by checking all pairs for difference 2. */
    for (size_t j = 0; j < count; ++j) {
        for (size_t k = j + 1; k < count; ++k) {
            seq_t a = divs[j];
            seq_t b = divs[k];
            seq_t diff = a > b ? a - b : b - a;
            if (diff == (seq_t)2) {
                twin[j] = true;
                twin[k] = true;
            }
        }
    }

    /* Sum non‑twin divisors. */
    seq_t sum = (seq_t)0;
    for (size_t j = 0; j < count; ++j)
        if (!twin[j])
            sum += divs[j];

    return sum;
}
