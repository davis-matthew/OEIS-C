/*
 * A385848  Numbers not of the form 2^m*k + m + 1, for integers m, k >= 1.
 * https://oeis.org/A385848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 5504, u32 n <= 5504, u64 n <= 5504,
 *            u128 n <= 5504, big any n
 * Validated: standard, pass, 73 terms through n = 5476 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = nth positive integer not representable as 2^m*k + m + 1 for m,k>=1 */
#include <stdbool.h>

static bool is_representable(seq_t N) {
    // Check whether N can be written in the form 2^m*k + m + 1 with m,k>=1.
    seq_t pow = (seq_t)2;          // 2^1
    idx_t m   = 1;
    for (;;) {
        // If m+1 exceeds N, no larger m can work.
        if (N < ((seq_t)m + 1)) break;
        seq_t diff = N - ((seq_t)m + 1);
        // For a representation we need diff >= pow.
        if (diff < pow) break;
        // If diff is a multiple of pow, N is representable.
        if ((diff & (pow - (seq_t)1)) == (seq_t)0) return true;
        // Prepare next m: stop if the next power would exceed N.
        if (pow > N / 2) break;
        pow <<= 1;
        m++;
    }
    return false;
}

seq_t a(idx_t n) {
    // Compute the nth term (n >= 1).
    idx_t count = 0;
    seq_t N = (seq_t)1;
    while (count < n) {
        if (!is_representable(N)) {
            ++count;
            if (count == n) return N;
        }
        ++N;
    }
    return (seq_t)0;  // should never reach here for valid input
}
