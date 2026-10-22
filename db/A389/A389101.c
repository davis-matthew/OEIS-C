/*
 * A389101  Array read by ascending antidiagonals: A(n, k) = gcd(n^k, k).
 * https://oeis.org/A389101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = gcd( i^j , j ) where (i,j) are the coordinates obtained *
 *            by reading the array A(i,j) = gcd(i^j , j) along ascending
 *            antidiagonals * with i decreasing within each diagonal (s = i +
 *            j)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute x^e mod m, with all arguments non‑negative.
   The algorithm uses only multiplication and modulo operations,
   keeping intermediate values < m. */
static seq_t powmod_seq(seq_t x, idx_t e, seq_t m)
{
    seq_t result = 1 % m;
    seq_t base = x % m;
    while (e) {
        if (e & 1) result = (seq_t)((result * base) % m);
        base = (seq_t)((base * base) % m);
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the diagonal sum s such that the total number of terms up to
     * that diagonal exceeds n.  The count of terms up to sum s is
     * T(s) = (s+1)(s+2)/2.  We use unsigned __int128 for the
     * intermediate multiplication to avoid overflow. */
    idx_t s = 0;
    while ((unsigned __int128)(s + 1) * (s + 2) / 2 <= n)
        ++s;

    /* Number of terms preceding this diagonal */
    idx_t prev = (idx_t)((unsigned __int128)s * (s + 1) / 2);
    /* Offset within the current diagonal (0 … s) */
    idx_t offset = n - prev;

    /* Coordinates (i, j) for this position */
    idx_t i = s - offset;
    idx_t j = offset;

    /* When j == 0 we have gcd(i^0, 0) = 1 (definition uses 0^0 = 1). */
    if (j == 0)
        return 1;

    seq_t mod = (seq_t)j;
    seq_t pow_val = powmod_seq((seq_t)i, j, mod);
    return gcd(pow_val, (seq_t)j);
}
