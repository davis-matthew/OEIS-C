/*
 * A331804  a(n) is the largest positive integer occurring, when written in
 *          binary, as a substring in both binary n and its reversal
 *          (A030101(n)).
 * https://oeis.org/A331804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the largest integer that appears as a binary substring in
 *            both the binary representation of n and its bitwise reversal
 * Range:     u8 n <= 256, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  seq_t, idx_t are defined by the build.
    The function works for any n >= 0. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Convert the index to a 64‑bit unsigned value */
    uint64_t N = (uint64_t)n;
    /* Determine the bit length of N (N > 0) */
    int len = 0;
    uint64_t tmp = N;
    while (tmp) { len++; tmp >>= 1; }

    /* Compute the reversal of N within its bit length */
    uint64_t rev = 0;
    for (int i = 0; i < len; ++i)
        rev = (rev << 1) | ((N >> i) & 1ULL);

    /* Collect all binary substrings of N (as integer values).
       The maximum number of substrings for 64 bits is 2080. */
    const int MAX_SUB = 2080;
    uint64_t subs[MAX_SUB];
    int subsCnt = 0;

    for (int i = 0; i < len; ++i) {
        uint64_t val = 0;
        for (int j = i; j < len; ++j) {
            int bitpos = len - 1 - j;          /* bit index from MSB */
            val = (val << 1) | ((N >> bitpos) & 1ULL);
            subs[subsCnt++] = val;
        }
    }

    /* Find the largest value that appears as a substring of both N and rev */
    uint64_t best = 0;
    for (int i = 0; i < len; ++i) {
        uint64_t val = 0;
        for (int j = i; j < len; ++j) {
            int bitpos = len - 1 - j;
            val = (val << 1) | ((rev >> bitpos) & 1ULL);

            /* Check if val occurs among the substrings of N */
            for (int k = 0; k < subsCnt; ++k) {
                if (subs[k] == val) {
                    if (val > best) best = val;
                    break;                     /* found in N, stop inner search */
                }
            }
        }
    }

    return (seq_t)best;
}
