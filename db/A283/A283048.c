/*
 * A283048  a(n) = (6*n^2)!/((n^2)!^6).
 * https://oeis.org/A283048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^2)! / ((n^2)!^6)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline int getbit(const uint8_t *arr, uint64_t idx) {
    return (arr[idx >> 3] >> (idx & 7)) & 1;
}
static inline void setbit(uint8_t *arr, uint64_t idx) {
    arr[idx >> 3] |= (1 << (idx & 7));
}

/* Compute the multinomial coefficient (6k)!/(k!)^6 by multiplying 6 binomials */
static seq_t binomial(idx_t N, idx_t K) {
    if (K > N) return 0;
    if (K > N - K) K = N - K;
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res = (res * (N - K + i)) / i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;

    idx_t k = n * n;                /* k = n^2 */
    idx_t N = 6 * k;                /* N = 6*k */
    seq_t result = 1;

    idx_t remain = N;
    for (idx_t face = 0; face < 6; ++face) {
        seq_t c = binomial(remain, k);
        result *= c;
        remain -= k;
    }
    return result;
}
