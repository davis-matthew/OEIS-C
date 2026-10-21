/*
 * A245314  Maximum frustration of complete bipartite graph K(n,7).
 * https://oeis.org/A245314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   piecewise based on floor(154/64*n) with special adjustments *
 *            a(n) = floor(154/64*n) - 2 if n = 1, 3, 5, 10, 26 * a(n) =
 *            floor(154/64*n) - 1 if n = 2, 14, 17, 18, 36, 49 * a(n) =
 *            floor(154/64*n) if n ≡ 7,12,14,16,17,22,24,26,27 (mod 32) or * n
 *            ≡ 3,8,18,34,36,38,43,51,63 (mod 64) * otherwise a(n)...
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>


seq_t a(idx_t n)
{
    seq_t base = ((seq_t)n * 154ULL) / 64ULL;

    /* subtract 2 cases */
    if (n == 1ULL || n == 3ULL || n == 5ULL ||
        n == 10ULL || n == 26ULL)
    {
        return base - 2;
    }

    /* subtract 1 cases that override mod conditions */
    if (n == 2ULL || n == 14ULL || n == 17ULL || n == 18ULL ||
        n == 36ULL || n == 49ULL)
    {
        return base - 1;
    }

    /* mod‑32 and mod‑64 checks for no subtraction */
    idx_t mod32 = n & 31ULL;   /* n % 32 */
    idx_t mod64 = n & 63ULL;   /* n % 64 */

    switch (mod32) {
        case 7: case 12: case 14: case 16: case 17:
        case 22: case 24: case 26: case 27:
            return base;
    }
    switch (mod64) {
        case 3: case 8: case 18: case 34: case 36:
        case 38: case 43: case 51: case 63:
            return base;
    }

    return base - 1;
}
