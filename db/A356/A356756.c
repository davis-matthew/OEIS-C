/*
 * A356756  Positive integers m such that x^2 + x + m contains at least m/2
 *          prime numbers for x = 1, 2, ..., m.
 * https://oeis.org/A356756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search for positive odd m such that
 * among x = 1..m the number of primes of x^2 + x + m is at least m/2. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cache of discovered terms */
    static seq_t cache[20];     /* room for the 15 known terms + safety */
    static size_t count = 0;    /* number of terms stored */
    static bool initialized = false;

    if (!initialized) {
        /* Search for terms up to a reasonable limit. All known terms
         * lie below 20000. */
        const idx_t LIMIT = 20000;
        for (idx_t m = 1; m <= LIMIT; m += 2) {    /* m must be odd */
            idx_t required = (m + 1) / 2;          /* ceil(m/2) */
            idx_t found = 0;
            for (idx_t x = 1; x <= m; ++x) {
                /* Compute x^2 + x + m using 64‑bit arithmetic to avoid
                 * overflow in the intermediate step when seq_t is small. */
                uint64_t val64 = (uint64_t)x * x + x + m;
                seq_t val = (seq_t)val64;
                if (is_prime_big(val))
                    ++found;
                /* Early exit: even if all remaining values were prime,
                 * we cannot reach the required count. */
                if (found + (m - x) < required)
                    break;
            }
            if (found >= required) {
                cache[count++] = (seq_t)m;
                if (count >= 15)           /* only 15 terms known < 20000 */
                    break;
            }
        }
        initialized = true;
    }

    if (n == 0 || n > count)
        return (seq_t)0;                 /* undefined for out of bounds */
    return cache[n - 1];
}
