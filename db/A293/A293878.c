/*
 * A293878  Numbers having '18' as substring of their digits / decimal
 *          expansion.
 * https://oeis.org/A293878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 2989, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A293878: numbers with substring '18'
 * This implementation scans natural numbers until the nth occurrence
 * of digits '18' is found.
 */
#include <stdbool.h>

/* Return the nth number (1‑based) that contains the substring "18". */
seq_t a(idx_t n) {
    /* Count of numbers seen that contain "18". */
    idx_t count = 0;
    /* Current candidate natural number */
    idx_t candidate = 1;
    while (1) {
        /* Check if candidate contains "18". */
        idx_t tmp = candidate;
        bool has18 = false;
        while (tmp >= 10) {
            /* Get the last two digits. */
            if ((tmp % 10) == 8 && ((tmp / 10) % 10) == 1) {
                has18 = true;
                break;
            }
            tmp /= 10;
        }
        if (has18) {
            ++count;
            if (count == n) {
                return (seq_t)candidate; /* candidate fits into seq_t for valid n */
            }
        }
        ++candidate;
    }
}
