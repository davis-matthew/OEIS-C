/*
 * A350325  Binomial transform of A339399(n).
 * https://oeis.org/A350325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * A339399(n-k+1)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

static inline seq_t A339399_cum_floor_half(idx_t M) {
    if (M == 0) return (seq_t)0;
    seq_t t = (seq_t)(M >> 1);
    if ((M & 1ULL) == 0) {
        return t * (t + 1);
    } else {
        return (t + 1) * (t + 1);
    }
}

static inline idx_t A339399_floor_sqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            if (mid == 0) return 0;
            hi = mid - 1;
        }
    }
    return hi;
}

static seq_t A339399_a(idx_t n) {
    if (n == 0) return (seq_t)0;

    seq_t nseq = (seq_t)n;
    idx_t two_n = n + n;

    if ((n & 1ULL) == 0) {
        idx_t sqrt1  = A339399_floor_sqrt_u64(two_n - 2);
        idx_t sqrt2  = A339399_floor_sqrt_u64(two_n - 3);
        idx_t M = (sqrt2 > 0) ? (sqrt2 - 1) : 0;
        seq_t sum = A339399_cum_floor_half(M);

        seq_t term1 = (seq_t)sqrt1 + 1;
        seq_t term2 = (nseq - 2 * sum) >> 1;
        return term1 - term2;
    } else {
        idx_t sqrt2 = A339399_floor_sqrt_u64(two_n - 1);
        idx_t M = (sqrt2 > 0) ? (sqrt2 - 1) : 0;
        seq_t sum = A339399_cum_floor_half(M);

        seq_t val = (nseq + 1 - 2 * sum) >> 1;
        return val;
    }
}

seq_t a(idx_t n) {
    seq_t *row = mem_new(seq_t, (size_t)n + 1);
    row[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        for (idx_t j = i; j >= 1; j--) {
            row[j] = row[j] + row[j - 1];
        }
    }
    seq_t total = 0;
    for (idx_t k = 0; k <= n; k++) {
        total += row[k] * A339399_a(n - k + 1);
    }
    mem_free(row);
    return total;
}
