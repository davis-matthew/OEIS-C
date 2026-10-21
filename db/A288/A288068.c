/*
 * A288068  Repdigits in base 10 which are Brazilian numbers.
 * https://oeis.org/A288068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=7, a(2)=8. For n≥3, a(n) is the (n‑2)th repdigit in base 10
 *            that is a Brazilian number, i.e. all repdigits ≥12. The
 *            repdigits are enumerated in increasing order:
 *            22,33,…,99,111,222,…
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility needed for ten‑power calculation */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)8;

    /* m = position among the Brazilian repdigits (starting from 1 for 22) */
    idx_t m = n - 2;

    /* First 8 positions are the 2‑digit repdigits 22…99 */
    if (m <= 8) {
        idx_t L = 2;                     /* length */
        seq_t d = (seq_t)(m + 1);        /* digit 2..9 */
        seq_t tenpow = pow10(L);         /* 10^L  */
        return ((tenpow - 1) * d) / 9;   /* d*(10^L–1)/9  */
    }

    /* Positions beyond the 2‑digit block: 3‑digit and larger repdigits */
    idx_t new_m = m - 8;                 /* offset after 2‑digit block */

    /* Determine length L and digit d from new_m */
    idx_t L = ((new_m - 1) / 9) + 3;     /* length: 3,4,…   */
    seq_t d = ((new_m - 1) % 9) + 1;     /* digit: 1..9     */

    seq_t tenpow = pow10(L);             /* 10^L  */
    return ((tenpow - 1) * d) / 9;       /* d*(10^L–1)/9  */
}
