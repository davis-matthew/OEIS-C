/*
 * A393248  a(n) is the period length of the Ducci sequence, where the initial
 *          vector consists of the binary digits of n.
 * https://oeis.org/A393248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the period length of the Ducci sequence on the binary
 *            digits of n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static inline seq_t ducci_step(seq_t v, idx_t k, seq_t mask)
{
    /* Circular left shift by one then XOR with original. */
    seq_t rot = ((v << 1) | (v >> (k - 1))) & mask;
    return v ^ rot;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 per OEIS offset */
    seq_t v = (seq_t)n;

    /* Compute the number of bits of the starting vector. */
    idx_t k = 0;
    seq_t tmp = v;
    while (tmp) {
        k++;
        tmp >>= 1;
    }

    /* Mask to keep only the k lower bits. */
    seq_t mask = ((seq_t)1 << k) - 1;

    /* Floyd's cycle–finding algorithm. */
    seq_t tortoise = ducci_step(v, k, mask);
    seq_t hare = ducci_step(ducci_step(v, k, mask), k, mask);

    while (tortoise != hare) {
        tortoise = ducci_step(tortoise, k, mask);
        hare = ducci_step(ducci_step(hare, k, mask), k, mask);
    }

    /* Find the length λ of the cycle. */
    idx_t lam = 1;
    hare = ducci_step(tortoise, k, mask);
    while (hare != tortoise) {
        hare = ducci_step(hare, k, mask);
        lam++;
    }

    return (seq_t)lam;
}
