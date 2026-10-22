/*
 * A302563  Numbers whose digital root is equal to their number of digits.
 * https://oeis.org/A302563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, base
 * Formula:   a(n) = first k‑digit number ≡ k (mod 9) + 9 · (n - a_{k‑1} - 1)
 *            where a_{k‑1} = (10^k - 1)/9 is the number of terms with fewer
 *            than k digits. The sequence is the increasing list of all
 *            integers whose digital root equals their number of decimal
 *            digits
 * Range:     u8 n <= 29, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* k is the digit length of the n‑th term */
    idx_t k = 1;
    idx_t pow10_i = 1;          /* 10^(k-1) – as an idx_t (safe for counting) */
    seq_t lo = 1;               /* 10^(k-1) – as a seq_t (the first k‑digit number) */
    idx_t cum = 0;              /* how many terms before the current block */

    /* find the block (k digits) that contains the n-th term */
    while (1) {
        idx_t block_count = pow10_i;          /* 10^(k-1) terms in this block */
        if (n <= cum + block_count) {
            break;                            /* current k is the block */
        }
        cum += block_count;                   /* advance to next block */
        k++;
        pow10_i *= 10;
        lo *= 10;
    }

    /* position of the term inside its block (0‑based) */
    idx_t idx_in_block = n - cum - 1;

    /* find the first number in this block that satisfies the congruence */
    seq_t r = (seq_t)(k % 9);                 /* desired residue (k%9, 0 for k%9==0) */
    seq_t lo_mod = lo % 9;
    seq_t delta = (r + 9 - lo_mod) % 9;       /* how far to the first valid number */
    seq_t first = lo + delta;

    /* compute the n‑th term */
    return first + (seq_t)9 * (seq_t)idx_in_block;
}
