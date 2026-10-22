/*
 * A361477  a(n) is the number of integers whose binary expansions have the
 *          same multiset of run-lengths as that of n.
 * https://oeis.org/A361477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 5196, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the number of permutations of the multiset of binary
 * run‑lengths of n.  The binary representation is taken without leading
 * zeros; n = 0 is a special case with a(0) = 1. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the empty binary string n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute factorials up to 64 (maximum run count for a 64‑bit integer). */
    static seq_t fact[65];
    static bool init = false;
    if (!init) {
        fact[0] = (seq_t)1;
        for (idx_t i = 1; i <= 64; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        init = true;
    }

    /* Find the position of the most significant set bit (0‑based). */
    idx_t msb = 63 - (idx_t)__builtin_clzll((uint64_t)n);

    /* Count the run lengths.  run_len stores the current run length. */
    unsigned char length_count[65] = {0};   /* length_count[l] = number of runs of length l */
    idx_t run_count = 0;                   /* total number of runs */

    int prev_bit = (n >> msb) & 1;
    idx_t run_len = 1;

    for (idx_t pos = msb; pos > 0; --pos) {
        int bit = (n >> (pos - 1)) & 1;
        if (bit == prev_bit) {
            ++run_len;
        } else {
            ++run_count;
            if (run_len <= 64)
                ++length_count[run_len];
            else
                /* run_len cannot exceed 64 for a 64‑bit n; no action needed. */
                ;
            run_len = 1;
            prev_bit = bit;
        }
    }
    /* Add the final run. */
    ++run_count;
    if (run_len <= 64)
        ++length_count[run_len];

    /* Compute the multinomial coefficient: run_count! / Π (count[l]!). */
    seq_t result = fact[run_count];
    for (idx_t l = 1; l <= 64; ++l) {
        if (length_count[l] > 1)
            result /= fact[length_count[l]];
    }
    return result;
}
