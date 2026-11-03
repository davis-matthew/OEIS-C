/*
 * A327107  BII-numbers of set-systems with minimum vertex-degree > 1.
 * https://oeis.org/A327107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = BII‑number of the n‑th set‑system whose minimum
   vertex‑degree is > 1.  The sequence is obtained by testing every
   integer for the property described above. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Test whether an integer cand yields a valid set‑system.
   All vertices must appear in at least two of the generated sets. */
static bool ok_for(uint64_t cand)
{
    if (cand == 0) return false;

    /* Collect positions of 1‑bits (1‑based) of cand. */
    int pos[64];
    int plen = 0;
    for (int i = 0; cand; ++i) {
        if (cand & 1ULL) pos[plen++] = i + 1;
        cand >>= 1ULL;
    }

    /* Frequency of each vertex in the resulting set‑system. */
    int freq[65] = {0};
    int maxbit = 0;
    for (int k = 0; k < plen; ++k) {
        uint64_t val = pos[k];
        int b = 1;
        while (val) {
            if (val & 1ULL) {
                ++freq[b];
                if (b > maxbit) maxbit = b;
            }
            val >>= 1ULL;
            ++b;
        }
    }

    /* All vertices must appear in at least two sets. */
    for (int v = 1; v <= maxbit; ++v)
        if (freq[v] == 1) return false;

    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n = 0 */

    idx_t count = 0;
    uint64_t cand = 1;
    while (1) {
        if (ok_for(cand)) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
        ++cand;
    }
}
