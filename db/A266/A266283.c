/*
 * A266283  Binary representation of the n-th iteration of the "Rule 13"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = (100*10^n - 1)/99 for n even * a(n) = (10*100^n -
 *            (1000*10^n-1)/11 - 1)/9 for n odd * (i.e. a(n) = (10*10^(2n+1) -
 *            (10^(n+3)-1)/11 - 1)/9) * These formulas are integer‑valued for
 *            all n ≥ 0
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Even indices */
    if ((n & 1ULL) == 0) {
        seq_t pow10n = big_pow(10, (seq_t)n);          /* 10^n   */
        seq_t num    = (seq_t)100 * pow10n - 1;       /* 100*10^n - 1 */
        return num / (seq_t)99;                       /* divide by 99 */
    }
    /* Odd indices */
    else {
        seq_t t1 = big_pow(10, (seq_t)(2*n + 1));      /* 10^(2n+1) = 10*100^n */
        seq_t t2 = (big_pow(10, (seq_t)(n + 3)) - 1) / (seq_t)11; /* (1000*10^n-1)/11 */
        seq_t temp = t1 - t2 - 1;
        return temp / (seq_t)9;                       /* divide by 9 */
    }
}
