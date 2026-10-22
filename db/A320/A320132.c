/*
 * A320132  Number of integer solutions to a^2 + b^2 + 2*c^2 + 10*d^2 = n.
 * https://oeis.org/A320132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to a^2 + b^2 + 2*c^2 + 10*d^2
 *            = n
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* local integer sqrt for uint64_t */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (n > 0) ? (n >> 1) + 1 : 0, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* pre‑compute the number of (a,b) pairs with a^2+b^2 = r up to N */
static void init_rcount(idx_t N, seq_t **rcount_out)
{
    size_t sz = (size_t)N + 1;
    seq_t *rcount = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!rcount) { *rcount_out = NULL; return; }

    seq_t *countA = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!countA) { free(rcount); *rcount_out = NULL; return; }

    idx_t max_s = isqrt_u64(N);
    for (idx_t s = 0; s <= max_s; ++s) {
        idx_t sq = s * s;
        countA[sq] = (s == 0 ? (seq_t)1 : (seq_t)2);
    }

    for (idx_t i = 0; i <= N; ++i) {
        if (countA[i] == 0) continue;
        for (idx_t j = 0; j <= N - i; ++j) {
            if (countA[j] == 0) continue;
            rcount[i + j] += countA[i] * countA[j];
        }
    }

    free(countA);
    *rcount_out = rcount;
}

/* memoised table of rcount values */
static seq_t *rcount_table = NULL;
static idx_t rcount_size   = 0;

seq_t a(idx_t n)
{
    if (n + 1 > rcount_size) {
        seq_t *new_table = NULL;
        init_rcount(n, &new_table);
        free(rcount_table);
        rcount_table = new_table;
        rcount_size   = n + 1;
    }

    seq_t total = 0;

    idx_t max_d = isqrt_u64(n / 10);
    for (idx_t d = 0; d <= max_d; ++d) {
        idx_t termD   = d * d * 10U;
        seq_t mult_d  = (d == 0 ? (seq_t)1 : (seq_t)2);
        idx_t rem_d   = n - termD;

        idx_t max_c = isqrt_u64(rem_d / 2);
        for (idx_t c = 0; c <= max_c; ++c) {
            idx_t termC   = c * c * 2U;
            seq_t mult_c  = (c == 0 ? (seq_t)1 : (seq_t)2);
            idx_t rem     = rem_d - termC;

            total += rcount_table[rem] * (mult_d * mult_c);
        }
    }

    return total;
}
