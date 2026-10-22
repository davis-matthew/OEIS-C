/*
 * A343336  Factors of alternators which produce least alternating proper
 *          multiples.
 * https://oeis.org/A343336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the least factor q > 1 such that q*n is an alternating
 *            integer. Returns 0 if n is a multiple of 20
 * Range:     u8 n <= 21, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Return 1 if x has alternating digit parities (single digit always qualifies). */
static bool is_alternating(seq_t x)
{
    if (x < 10) return true;           /* single digit is alternating */
    int prev = (int)(x % 10) & 1;       /* parity (0=even,1=odd) of least significant digit */
    x /= 10;
    while (x != 0) {
        int curr = (int)(x % 10) & 1;
        if (curr == prev) return false; /* consecutive digits with same parity */
        prev = curr;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* If n is a multiple of 20, it is not an alternator. */
    if ((n % 20) == 0) return 0;

    /* Search for the smallest q > 1 such that q*n is alternating. */
    for (idx_t q = 2; ; ++q) {
        seq_t prod = (seq_t)n * (seq_t)q;
        if (is_alternating(prod)) return (seq_t)q;
    }
}
