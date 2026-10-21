/*
 * A280512  Index sequence of the Thue-Morse sequence (A010060, using offset
 *          1) as a reverse block-fractal sequence.
 * https://oeis.org/A280512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the first (offset 1) index at which the reversal of the first
   n terms of the Thue–Morse sequence occurs.
   The algorithm builds the reversed pattern of length n
   and performs a KMP search over the infinite Thue–Morse sequence.
   Complexity is O(n + a(n)), which is fast enough for n ≤ 10 000. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Return the nth term in the sequence A280512 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)   /* out of range; sequence is defined only for n≥1 */
        return 0;

    /* Allocate pattern buffer and build the reversal
       of the first n Thue‑Morse bits. */
    uint8_t *pat = (uint8_t *)malloc((size_t)n);
    if (!pat)
        return 0;   /* OOM – caller will treat as failure */

    for (idx_t i = 0; i < n; ++i) {
        /* Thue–Morse bit s(k) = parity(popcount(k-1)) */
        idx_t v = n - 1 - i;   /* position in the original prefix */
        pat[i] = (uint8_t)__builtin_parityll((unsigned long long)v);
    }

    /* Build prefix function for KMP. */
    idx_t *pi = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!pi) {
        free(pat);
        return 0;
    }

    pi[0] = 0;
    idx_t j = 0;
    for (idx_t i = 1; i < n; ++i) {
        while (j > 0 && pat[i] != pat[j])
            j = pi[j - 1];
        if (pat[i] == pat[j])
            ++j;
        pi[i] = j;
    }

    /* KMP search over the Thue–Morse sequence. */
    seq_t pos = 1;   /* position in Thue–Morse (1‑based) */
    j = 0;
    while (1) {
        uint8_t t = (uint8_t)__builtin_parityll((unsigned long long)(pos - 1));
        while (j > 0 && t != pat[j])
            j = pi[j - 1];
        if (t == pat[j])
            ++j;
        if (j == n) {
            seq_t start = pos - n + 1;
            free(pat);
            free(pi);
            return start;
        }
        ++pos;
    }
}
