/*
 * A252664  Minimal nontrivial undulant (A046075) divisible by n, or 0 if no
 *          undulant is divisible by n.
 * https://oeis.org/A252664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   find minimal nontrivial undulant (alternating digits a,b, a≠b,
 *            a≠0) divisible by n, or 0 if none exists
 * Range:     u8 n <= 4, u16 n <= 98, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 336, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Compute a minimal undulant divisible by n. */
    const seq_t modN = (seq_t)n;
    /* Enumerate all (a,b) pairs with 1≤a≤9, 0≤b≤9, a≠b. */
    uint8_t pa[81];
    uint8_t pb[81];
    int cnt = 0;
    for (uint8_t a = 1; a <= 9; ++a) {
        for (uint8_t b = 0; b <= 9; ++b) {
            if (a == b) continue;
            pa[cnt] = a;
            pb[cnt] = b;
            ++cnt;
        }
    }
    /* residues[i] holds the residue modulo n of the current undulant
       of length len for pair i. Initialize for length 2. */
    seq_t residues[81];
    for (int i = 0; i < 81; ++i) {
        residues[i] = ((seq_t)pa[i] * 10 + pb[i]) % modN;
    }

    /* Upper bound on the length to search:
       after n steps the sequence of residues repeats, so if none
       between lengths 3 and n+2 we cannot find any. Ensure at
       least 3. */
    idx_t upperLen = (n > 2) ? (n + 2) : 3;
    for (idx_t len = 3; len <= upperLen; ++len) {
        bool found = false;
        uint8_t best_a = 10;            /* sentinel > max digit */
        uint8_t best_b = 10;
        bool odd = (len & 1);           /* true if len is odd => next digit a */
        for (int i = 0; i < 81; ++i) {
            uint8_t digit = odd ? pa[i] : pb[i];
            residues[i] = (residues[i] * 10 + digit) % modN;
            if (residues[i] == 0) {
                if (!found ||
                    pa[i] < best_a ||
                    (pa[i] == best_a && pb[i] < best_b)) {
                    found = true;
                    best_a = pa[i];
                    best_b = pb[i];
                }
            }
        }
        if (found) {
            /* Build the actual undulant number. */
            seq_t val = 0;
            for (idx_t k = 0; k < len; ++k) {
                uint8_t d = (k & 1) ? best_b : best_a;
                val = val * 10 + d;
            }
            return val;
        }
    }
    return 0;   /* no undulant exists for this n */
}
