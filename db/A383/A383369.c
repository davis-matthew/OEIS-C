/*
 * A383369  Population of elementary triangular automaton rule 90 at
 *          generation n, starting from a lone 1 cell at generation 0.
 * https://oeis.org/A383369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 4 For n ≥ 2: if n is even (n = 2k): if k is
 *            even: a(n) = a(k) else if k mod 4 == 1: a(n) = 3 * a(k) / 2
 *            else: a(n) = 2 * a(k) if n is odd (n = 2k + 1): a(n) = (k is
 *            even ? 4 : 2) * a(n – 1)
 * Range:     u8 n <= 22, u16 n <= 382, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 81 terms through n = 9498 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t recurse(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    if ((n & 1) == 0) {               /* n is even */
        idx_t k = n >> 1;
        seq_t v = recurse(k);
        if ((k & 1) == 0)                 return v;          /* k even */
        if ((k & 3) == 1)                 return v * 3 / 2;  /* k ≡ 1 (mod 4) */
        return v * 2;                    /* k ≡ 3 (mod 4) */
    } else {                            /* n is odd, n = 2k + 1 */
        idx_t k = n >> 1;                /* k = ⌊n/2⌋ = (n–1)/2 */
        seq_t v = recurse(k << 1);       /* a(2k) = a(n–1) */
        return (k & 1) ? v * 2 : v * 4;   /* k odd → 2·v, k even → 4·v */
    }
}

seq_t a(idx_t n)
{
    return recurse(n);
}
