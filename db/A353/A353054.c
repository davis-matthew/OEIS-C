/*
 * A353054  Numbers k such that placing the last digit first gives 2k+1.
 * https://oeis.org/A353054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   k = (d*(10^L - 1) - 10)/19 where L is the number of digits of k
 *            and d is its last digit
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* residues: L modulo 18 for the 7 distinct patterns */ 
    static const idx_t residues[7] = {4ULL, 5ULL, 8ULL, 10ULL, 12ULL, 13ULL, 14ULL};
    /* corresponding last digits d for each pattern */ 
    static const unsigned dvals[7] =    {2,    5,    3,    6,    8,    4,    7};

    /* Position within the repeating 7‑term cycle */ 
    idx_t cycle = (n - 1) / 7;
    idx_t idx   = (n - 1) % 7;

    /* Length of the number k (digits) and its last digit d */ 
    idx_t L = residues[idx] + 18ULL * cycle;
    unsigned d = dvals[idx];

    /* Compute 10^L and then k by the formula above */
    seq_t pow10L = pow10(L);
    seq_t t = pow10L - 1;   /* 10^L - 1  */
    t = t * d;              /* d * (10^L - 1) */
    t = t - 10;             /* subtract 10   */
    t = t / 19;             /* divide by 19  */

    return t;
}
