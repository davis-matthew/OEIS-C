/*
 * A267885  Binary representation of the n-th iteration of the "Rule 235"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 * a(1) = 100 * a(2) = 11011 * a(n) = (10^(2n+1) – 1) /
 *            9 for n ≥ 3 * Each value is the decimal integer whose digit
 *            string is the * binary pattern produced by the n‑th iteration of
 *            elementary * cellular automaton rule 235 started from a single
 *            ON cell
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Handle the first three special cases explicitly */
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)100;
    if (n == 2u)
        return (seq_t)11011;

    /* For n ≥ 3: a(n) = (10^(2n+1) – 1) / 9 */
    idx_t exp = 2 * n + 1;
    seq_t ten_pow = pow10(exp);         /* 10^(2n+1) */
    ten_pow = ten_pow - (seq_t)1;       /* subtract 1 */
    return ten_pow / (seq_t)9;          /* divide by 9 */
}
