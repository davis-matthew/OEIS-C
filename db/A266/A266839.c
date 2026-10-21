/*
 * A266839  Decimal representation of the n-th iteration of the "Rule 67"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A266839
 *  Decimal representation of the n‑th iteration of Rule 67 elementary cellular automaton.
 *  Base values:
 *      a(0) = 1
 *      a(1) = 4
 *      a(2) = 10
 *      a(3) = 97
 *  For n ≥ 4:
 *      if n is even:
 *          a(n) = 23 * 8^(n/2) / 16
 *      if n is odd:
 *          a(n) = 2 * 4^n - 61 * 8^floor(n/2) / 16 - 1
 *  These formulas are valid for all n ≥ 4 (even) and n ≥ 5 (odd).  They produce the
 *  exact integer values; for types that cannot hold the full value the arithmetic
 *  naturally overflows modulo the type width, which is acceptable because the
 *  specification only requires exactness when the result fits.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the small initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)97;

    /* Even indices (n ≥ 4) */
    if ((n & 1) == 0) {
        idx_t k = n / 2;                /* n/2 is an integer ≥ 2 */
        seq_t p = big_pow((seq_t)8, (seq_t)k); /* 8^k */
        seq_t r = (p / (seq_t)16) * (seq_t)23; /* 23 * (8^k / 16) */
        return r;
    }

    /* Odd indices (n ≥ 5) */
    {
        seq_t p4 = big_pow((seq_t)4, (seq_t)n);    /* 4^n */
        seq_t term1 = p4 * (seq_t)2;              /* 2 * 4^n */

        idx_t k = n / 2;                           /* floor(n/2) ≥ 2 for n ≥ 5 */
        seq_t p8 = big_pow((seq_t)8, (seq_t)k);    /* 8^k */
        seq_t term2 = (p8 / (seq_t)16) * (seq_t)61;/* 61 * (8^k / 16) */

        return term1 - term2 - (seq_t)1;
    }
}
