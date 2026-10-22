/*
 * A341766  a(n) = difference between the starting positions of the first
 *          digit of the binary representation of n, where n starts at its
 *          natural position in the string, and the second occurrence of the
 *          same string in the binary Champernowne string (starting at 0)
 *          011011100101110111100010011010... (cf. A030190).
 * https://oeis.org/A341766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 8191, u32 n <= 8501, u64 n <= 8501,
 *            u128 n <= 8501, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A341766:
 *   a(n) = (position of second occurrence of bin(n) in the binary Champernowne string)
 *          - (position of the natural occurrence of bin(n) in that string).
 *   Positions are 1‑based.  The natural occurrence of n is the one at its
 *   natural index in the concatenation 0,1,10,11,100,...
 *   This implementation scans the infinite binary Champernowne string
 *   using KMP until the second occurrence of the pattern is found.
 */

#include <stdint.h>
#include <stdbool.h>

/* returns bit length of x; for x==0 it returns 1 */
static inline idx_t bitlen_idx(idx_t x)
{
    idx_t len = 1;
    while (x >>= 1)
        ++len;
    return len;
}

seq_t a(idx_t n)
{
    /* pattern: binary representation of n */
    idx_t L = bitlen_idx(n);          /* length of pattern */
    unsigned char pat[64];            /* use 64 to cover all idx_t bits */

    for (idx_t i = 0; i < L; ++i)
        pat[i] = (n >> (L - 1 - i)) & 1;

    /* prefix function for KMP */
    unsigned char pi[64] = {0};
    for (idx_t i = 1; i < L; ++i) {
        int j = pi[i - 1];
        while (j > 0 && pat[i] != pat[j])
            j = pi[j - 1];
        if (pat[i] == pat[j])
            ++j;
        pi[i] = j;
    }

    /* position of natural occurrence (1‑based) */
    idx_t sum = 0;
    for (idx_t k = 0; k < n; ++k)
        sum += bitlen_idx(k);
    idx_t pos1 = sum + 1;  /* natural start position of n */

    /* scan the string until second occurrence is found */
    bool found_natural = false;
    idx_t match_start = 0;
    idx_t pos = 1;   /* current bit position (1‑based) */
    int j = 0;       /* current KMP state */

    for (idx_t num = 0; ; ++num) {
        idx_t len = bitlen_idx(num);
        for (int i = (int)len - 1; i >= 0; --i) {
            unsigned char bit = (num >> i) & 1;
            while (j > 0 && bit != pat[j])
                j = pi[j - 1];
            if (bit == pat[j])
                ++j;
            if (j == (int)L) {
                match_start = pos - L + 1;
                if (!found_natural) {
                    if (match_start == pos1)
                        found_natural = true;
                } else {
                    /* second occurrence found */
                    goto compute_diff;
                }
                j = pi[j - 1];
            }
            ++pos;
        }
    }

compute_diff:
    return (seq_t)(match_start - pos1);
}
