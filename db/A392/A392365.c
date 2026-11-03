/*
 * A392365  The cubefree numbers squared.
 * https://oeis.org/A392365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A004709(n)^2 (square of the n‑th cubefree number)
 * Range:     u8 n <= 14, u16 n <= 214, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 8787 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <limits.h>

/* integer cube root: largest k with k³ ≤ x */
static idx_t int_cube_root(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 cube = (__int128)mid * mid * mid;
        if (cube <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Return the n‑th squared cubefree number.
   n >= 1 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* safety for offset < 1 */

    idx_t count = 0;                 /* how many cubefree numbers found */
    idx_t candidate = 0;             /* integer candidate number */

    while (count < n) {
        ++candidate;                 /* next integer to test */

        /* test whether candidate is cubefree: no divisor p^3 */
        idx_t root = int_cube_root(candidate);
        int cubefree = 1;            /* assume true */

        for (idx_t p = 2; p <= root; ++p) {
            /* p^3 fits into seq_t for our small n (≤10000). */
            seq_t p3 = (seq_t)p * (seq_t)p * (seq_t)p;
            if ((seq_t)candidate % p3 == 0) {
                cubefree = 0;         /* found a cube divisor */
                break;
            }
        }

        if (cubefree) {
            ++count;
            if (count == n) {
                /* square the integer candidate (promote to seq_t). */
                seq_t val = (seq_t)candidate;
                return val * val;
            }
        }
    }
    /* should never reach here */
    return 0;
}
