/*
 * A277622  Integers such that the first part "a", divided by the penultimate
 *          digit "b", leaves the last digit "c" as remainder (see the Example
 *          section for a detailed explanation).
 * https://oeis.org/A277622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   For n ≥ 1 let g = n‑1. a = ⌊g/9⌋ + 1 (the “first part”) b = (g
 *            mod 9) + 1 (the penultimate digit) c = a mod b (the last digit)
 *            a(n) = a·100 + b·10 + c
 * Range:     u8 n <= 14, u16 n <= 5889, u32 n <= 8991, u64 n <= 8991,
 *            u128 n <= 8991, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the generic integer type. */
    seq_t n_seq = (seq_t)n;
    /* g = n-1. */
    seq_t g = n_seq - 1;
    /* Compute the three components. */
    seq_t a_part = g / 9 + 1;          /* first part of the number */
    seq_t b_digit = g % 9 + 1;         /* penultimate digit (1…9) */
    seq_t c_digit = a_part % b_digit;  /* remainder = last digit */
    /* Construct the full number. */
    return a_part * 100 + b_digit * 10 + c_digit;
}
