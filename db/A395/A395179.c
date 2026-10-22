/*
 * A395179  Maximum number of runs in the binary expansion of integers
 *          obtained by changing a single 0-bit to 1 in the binary
 *          representation of n considering bits up to and including the first
 *          leading 0.
 * https://oeis.org/A395179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = max { A005811(n + 2^k) : (n AND 2^k) == 0 and 0 ≤ k ≤
 *            ⌊log₂(n)⌋ + 1 }
 * Range:     u8 n <= 1318, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A005811(x) is the number of runs (maximal blocks of equal digits) in the binary representation of x. */

#include <stdint.h>
#include <stdbool.h>

/* Compute floor(log2(v)) for a 64‑bit unsigned integer. */
static inline unsigned floor_log2_idx(idx_t v)
{
    unsigned r = 0;
    while (v > 1) {
        v >>= 1;
        ++r;
    }
    return r;
}

/* Count the number of runs in the binary representation of a positive integer x. */
static inline seq_t count_runs(seq_t x)
{
    seq_t runs = 1;
    seq_t prev = x & 1;
    seq_t temp = x >> 1;
    while (temp) {
        seq_t bit = temp & 1;
        if (bit != prev) {
            ++runs;
            prev = bit;
        }
        temp >>= 1;
    }
    return runs;
}

/* OEIS A395179 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; the value for n=0 is unused, but safe guard. */

    unsigned log = floor_log2_idx(n);
    unsigned bound = log + 1;       /* include the leading zero position */
    seq_t best = 0;

    for (unsigned k = 0; k <= bound; ++k) {
        if (n & ((idx_t)1 << k))    /* already a 1‑bit; cannot flip */
            continue;

        idx_t shift = (idx_t)1 << k;
        seq_t m = (seq_t)n + (seq_t)shift;   /* new number after flipping bit k to 1 */

        seq_t runs = count_runs(m);
        if (runs > best)
            best = runs;
    }

    return best;
}
