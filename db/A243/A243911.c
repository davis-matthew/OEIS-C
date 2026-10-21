/*
 * A243911  Least number k such that n^k ends in two identical digits, or 0 if
 *          no such number exists.
 * https://oeis.org/A243911
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k ≥ 1 such that n^k ends in two identical digits,
 *            or 0 if no such k exists. Computation: iterate powers of n
 *            modulo 100; detect repetition of the residue. When a residue
 *            repeats we have entered a cycle with no solution, so return 0.
 *            The maximum period of powers modulo 100 is 100, so...
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;                      /* offset is 2; values below are irrelevant */

    const seq_t base = (seq_t)(n % 100);       /* only the last two digits of n matter */
    seq_t p = base;                            /* n^1 mod 100 */
    bool seen[100] = { false };                /* residues seen during the walk */

    for (idx_t k = 1; ; ++k) {
        /* Check if the two last decimal digits are identical */
        seq_t tens  = p / 10;
        seq_t units = p % 10;
        if (tens == units) return (seq_t)k;

        /* If this residue has been seen before we are in a cycle – no answer */
        unsigned idx = (unsigned)p;           /* p ∈ [0, 99] so cast is safe */
        if (seen[idx]) return 0;
        seen[idx] = true;

        /* Next power: multiply by base modulo 100 */
        p = (p * base) % ((seq_t)100);
    }
}
