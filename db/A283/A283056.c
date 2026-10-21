/*
 * A283056  Size of the smallest polyomino that admits a hole of size n.
 * https://oeis.org/A283056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 0 a(n) = 2 * ceil(2*sqrt(n)) + 3 for n >= 1 (this equals
 *            A027709(n) + 3)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for 64‑bit unsigned */
static inline idx_t _isqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 m2 = (unsigned __int128)mid * mid;
        if (m2 == n) return mid;
        if (m2 < n)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

/* Main function for the sequence */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    idx_t s = _isqrt_u64(n);                 /* floor(sqrt(n))          */
    unsigned __int128 s_sq = (unsigned __int128)s * s;
    unsigned __int128 sum  = s_sq + s;       /* s^2 + s                  */
    idx_t ceil_val;

    if (s_sq == n)                     /* sqrt(n) is integer       */
        ceil_val = 2 * s;
    else if ((unsigned __int128)n <= sum)
        ceil_val = 2 * s + 1;          /* d < 0.5                  */
    else
        ceil_val = 2 * s + 2;          /* d >= 0.5                  */

    seq_t perimeter = (seq_t)(2 * ceil_val); /* minimal perimeter       */
    return perimeter + 3;                  /* add the 3 extra cells    */
}
