/*
 * A320125  Number of integer solutions to a^2 + b^2 + 2*c^2 + 4*d^2 = n.
 * https://oeis.org/A320125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   count of integer solutions to a^2 + b^2 + 2*c^2 + 4*d^2 = n
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The implementation uses two auxiliary arrays.  The complexity
   is O(n · √n) for a call with argument n, which is well below the
   2‑second limit for n ≤ 10 000. */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root of n (floor(sqrt(n))) on nano‑kernal level. */
static inline idx_t int_sqrt(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __int128 sq = (__int128)mid * (__int128)mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n) {
    idx_t N = n;
    /* Allocate arrays for counts of a^2 + b^2 and 2*c^2 + 4*d^2. */
    seq_t *A = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *B = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!A || !B) {
        if (A) free(A);
        if (B) free(B);
        return 0;
    }

    /* Compute A[k] = # of (a,b) with a^2 + b^2 = k. */
    idx_t maxA = int_sqrt(N);
    for (idx_t a = 0; a <= maxA; ++a) {
        idx_t a2 = a * a;          /* a^2 fits in idx_t for n ≤ 10^8 */
        idx_t remA = N - a2;
        idx_t maxB = int_sqrt(remA);
        for (idx_t b = 0; b <= maxB; ++b) {
            idx_t valIdx = a2 + b * b;   /* ≤ N by construction */
            seq_t mult = 1;
            if (a != 0) mult <<= 1;   /* +/− a when a ≠ 0 */
            if (b != 0) mult <<= 1;   /* +/− b when b ≠ 0 */
            A[(size_t)valIdx] += mult;
        }
    }

    /* Compute B[k] = # of (c,d) with 2*c^2 + 4*d^2 = k. */
    idx_t maxD = int_sqrt(N / 4u);
    for (idx_t d = 0; d <= maxD; ++d) {
        idx_t d2 = d * d;
        idx_t remD = N - (4u * d2);
        idx_t maxC = int_sqrt(remD / 2u);
        for (idx_t c = 0; c <= maxC; ++c) {
            idx_t valIdx = 2u * c * c + 4u * d * d;   /* ≤ N */
            seq_t mult = 1;
            if (c != 0) mult <<= 1;   /* +/− c when c ≠ 0 */
            if (d != 0) mult <<= 1;   /* +/− d when d ≠ 0 */
            B[(size_t)valIdx] += mult;
        }
    }

    /* Convolution A * B gives the desired counts. */
    seq_t ans = 0;
    for (idx_t k = 0; k <= N; ++k)
        ans += A[(size_t)k] * B[(size_t)(N - k)];

    free(A);
    free(B);
    return ans;
}
