/*
 * A266589  Binary representation of the n-th iteration of the "Rule 37"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A266589: binary representation of the n‑th iteration of Rule 37.
   Formula (OEIS):
       a(0) = 1,
       a(1) = 10,
       a(2k) = 111 * 10^(2k-1)          (even n > 0)
       a(2k+1) = (10 * 100^(2k+1) - 99999 * 10^(2k-1) - 1) / 9   (odd n > 1) */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    /* Even indices */
    if ((n & 1) == 0) {
        seq_t res = pow10(n - 1);      /* 10^(n-1) */
        res *= (seq_t)111;             /* 111 * 10^(n-1) */
        return res;
    }

    /* Odd indices (n > 1) */
    idx_t two_n = n << 1;              /* 2n */
    seq_t term1 = pow10(two_n);        /* 10^(2n) = 100^n */
    term1 *= (seq_t)10;                /* 10 * 10^(2n) */
    seq_t term2 = pow10(n - 2);        /* 10^(n-2) */
    term2 *= (seq_t)99999;             /* 99999 * 10^(n-2) */
    seq_t num = term1 - term2;         /* numerator of the division */
    num -= (seq_t)1;
    return num / (seq_t)9;             /* divide by 9 */
}
