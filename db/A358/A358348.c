/*
 * A358348  Numbers k such that k == k^k (mod 9).
 * https://oeis.org/A358348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that k^k ≡ k (mod 9).
   Computed by iterating integers and testing the congruence condition. */
#include <stdbool.h>

/* returns true iff k^k ≡ k (mod 9) */
static inline bool good_k(idx_t k)
{
    idx_t r = k % 9;
    if (r == 0)                     /* multiples of 9   */
        return true;
    if (r == 1 || r == 4 || r == 7) /* residues 1,4,7   */
        return true;
    if (r == 8 && (k & 1ULL))       /* residue 8, odd k */
        return true;
    return false;
}

/* seq_t a(idx_t n)  – n-th term (n≥1) */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined, but safe */
        return (seq_t)0;

    idx_t k   = 1;
    idx_t cnt = 0;
    while (true) {
        if (good_k(k)) {
            ++cnt;
            if (cnt == n)
                return (seq_t)k;
        }
        ++k;
    }
}
