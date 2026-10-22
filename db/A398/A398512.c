/*
 * A398512  n is the start of a run of exactly a(n) consecutive numbers all
 *          containing a repeated digit.
 * https://oeis.org/A398512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 3996, u16 n <= 6000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   unique_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 0 if n has all distinct digits;
 * otherwise a(n) = m - n, where m is the first integer > n
 * whose decimal representation contains no repeated digits.
 * For n > 9876543210 the next distinct‑digit number does not exist,
 * so a(n) is defined here as 0 to prevent infinite loops.
 */

#include <stdbool.h>
#include "../utils/unique_digits.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    /* Maximum 10‑digit number with all distinct digits: 9876543210 */
    const seq_t max_unique = (seq_t)9876543210ULL;

    if (x > max_unique)
        return (seq_t)0;          /* safety guard for out‑of‑range indices */

    if (unique_digits(x))
        return (seq_t)0;          /* n itself is distinct‑digit */

    seq_t m = x + (seq_t)1;
    while (m <= max_unique) {
        if (unique_digits(m))
            break;
        ++m;
    }

    if (m > max_unique)
        return (seq_t)0;          /* no distinct‑digit number found */

    return m - x;
}
