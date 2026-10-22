/*
 * A354541  Number of ways to tile a double-hexagon strip of n hexagons, using
 *          single and double hexagons.
 * https://oeis.org/A354541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 74, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354541: Number of tilings of a double-hexagon strip of n hexagons.
   Base values: a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=8.
   For n ≥ 5 the following closed forms hold:
     a(3k)   = 4 * 6^(k-1)
     a(3k+1) = 8 * 6^(k-1)
     a(3k+2) = 12 * 6^(k-1)
   where k = n/3, (n-1)/3, or (n-2)/3 according to the residue of n modulo 3. */
#include <stddef.h>
/* No external on‑demand functions are required; a small loop computes 6^m. */
seq_t a(idx_t n) {
    static const seq_t base[5] = {1, 1, 2, 4, 8};
    if (n < 5) return base[n];

    idx_t k, i;
    seq_t pow6 = 1;
    seq_t ans;

    switch (n % 3) {
        case 0:  /* n = 3k, k ≥ 1 */
            k = n / 3;                /* k ≥ 1 */
            for (i = 0; i < k - 1; ++i) pow6 *= 6;
            ans = 4 * pow6;
            break;
        case 1:  /* n = 3k+1, k ≥ 1 */
            k = (n - 1) / 3;          /* k ≥ 1 */
            for (i = 0; i < k - 1; ++i) pow6 *= 6;
            ans = 8 * pow6;
            break;
        case 2:  /* n = 3k+2, k ≥ 1 */
            k = (n - 2) / 3;          /* k ≥ 1 */
            for (i = 0; i < k - 1; ++i) pow6 *= 6;
            ans = 12 * pow6;
            break;
    }
    return ans;
}
