/*
 * A370760  Table read by rows: row n is the unique primitive Pythagorean
 *          triple (a,b,c) such that a = prime(n).
 * https://oeis.org/A370760
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(2+3k) = (p^2-1)/2, * a(3+3k) = (p^2+1)/2, * a(4+3k) = p; *
 *            where p = prime(k+2) (A000040)
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* Make sure we don't underflow when n < 2 */
    if (n < 2) return 0;

    /* 0‑based offset into the flattened list of triples */
    idx_t m = n - 2;
    idx_t row_index = m / 3 + 2;     /* prime index for this row          */
    idx_t col = m % 3;               /* 0 → p, 1 → (p^2-1)/2, 2 → (p^2+1)/2 */

    uint32_t p32 = get_prime(row_index);  /* p fits in 32 bits */
    seq_t p = (seq_t)p32;
    seq_t val;

    if (col == 0) {
        val = p;
    } else if (col == 1) {
        val = (p * p - 1) / 2;
    } else { /* col == 2 */
        val = (p * p + 1) / 2;
    }

    return val;
}
