/*
 * A306925  Sum over all partitions of n into distinct parts of the bitwise
 *          XOR of the parts.
 * https://oeis.org/A306925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum over all partitions of n into distinct parts of the
 *            bitwise XOR of the parts
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 176, u64 n <= 653,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment. */

static int bitlen_u64(idx_t n) {
    int b = 0;
    while (n) { ++b; n >>= 1; }
    return b;
}

seq_t a(idx_t n) {
    /* Number of bits needed to represent n */
    int bits = bitlen_u64(n);
    if (n == 0) return 0;

    /* Allocate arrays */
    seq_t *cnt = NULL;
    seq_t *odd = NULL;

#if defined(__cplusplus)
    cnt = new seq_t[n + 1];
    odd = new seq_t[bits * (n + 1)];
#else
    cnt = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    odd = (seq_t*)malloc(bits * (n + 1) * sizeof(seq_t));
#endif

    /* Initialize arrays to zero */
    for (idx_t i = 0; i <= n; ++i) cnt[i] = 0;
    size_t odd_size = (size_t)bits * (n + 1);
    for (size_t i = 0; i < odd_size; ++i) odd[i] = 0;

    cnt[0] = 1;          /* Empty subset */

    /* Main DP */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t kbits = k;
        for (idx_t s = n; s >= k; --s) {
            seq_t from_cnt = cnt[s - k];
            cnt[s] += from_cnt;
            size_t idx_prev = s - k;
            size_t idx_cur  = s;
            for (int b = 0; b < bits; ++b) {
                seq_t old_odd = odd[(size_t)b * (n + 1) + idx_prev];
                if (kbits & ((idx_t)1 << b)) {
                    odd[(size_t)b * (n + 1) + idx_cur] += (from_cnt - old_odd);
                } else {
                    odd[(size_t)b * (n + 1) + idx_cur] += old_odd;
                }
            }
        }
    }

    /* Compute result from odd parity counts */
    seq_t result = 0;
    size_t idx_n = n;
    for (int b = 0; b < bits; ++b) {
        seq_t oc = odd[(size_t)b * (n + 1) + idx_n];
        result += oc << b;
    }

#if defined(__cplusplus)
    delete[] cnt;
    delete[] odd;
#else
    free(cnt);
    free(odd);
#endif

    return result;
}
