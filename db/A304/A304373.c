/*
 * A304373  Numbers n with additive persistence = 4.
 * https://oeis.org/A304373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 10, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A304373: numbers with additive persistence 4.
   This is equivalent to the sequence of positive integers whose
   digit sum is 199.  The numbers are listed in increasing order.
   The function returns the nth term (n ≥ 1). */

#include <stddef.h>
#include <stdint.h>

/* Pre‑computed DP: dp[t][s] = number of ways to fill t decimal
   digits (each 0–9) with sum s.  The values are capped at a large
   constant to avoid overflow; only values up to the required index
   are needed. */
#define SUM 199
#define MAX_LEN 200           /* sufficient for n ≤ 10 000 */
#define CAP 1000000000000ULL  /* > any index we will handle */

static uint64_t dp[MAX_LEN+1][SUM+1];
static int  dp_init = 0;

static void init_dp(void) {
    if (dp_init) return;
    dp_init = 1;
    dp[0][0] = 1;
    for (int t = 1; t <= MAX_LEN; ++t) {
        for (int s = 0; s <= SUM; ++s) {
            uint64_t total = 0;
            for (int d = 0; d <= 9; ++d) {
                if (s < d) break;
                uint64_t val = dp[t-1][s-d];
                if (total > CAP - val) { total = CAP; break; }
                total += val;
            }
            dp[t][s] = total;
        }
    }
}

/* Count how many numbers of a fixed length 'len' have digit sum SUM,
   with the first digit in 1..9.  If the count exceeds CAP it is
   returned as CAP. */
static uint64_t count_len(int len) {
    if (len <= 0) return 0;
    uint64_t total = 0;
    for (int d = 1; d <= 9; ++d) {
        int r = SUM - d;
        if (r < 0) break;
        uint64_t add = dp[len-1][r];
        if (total > CAP - add) { total = CAP; break; }
        total += add;
    }
    return total;
}

/* Return the nth number with digit sum SUM.
   n is 1‑based. */
seq_t a(idx_t n) {
    init_dp();
    if (n == 0) return (seq_t)0;      /* undefined, but guard */

    uint64_t k = (uint64_t)n;         /* index we are looking for */
    const int min_len = (SUM + 8)/9;  /* ceil(SUM/9) */

    int len = min_len;
    while (1) {
        uint64_t cnt = count_len(len);
        if (k > cnt) {
            k -= cnt;
            ++len;
        } else
            break;
    }

    seq_t res = 0;
    int sum_so_far = 0;
    for (int pos = 0; pos < len; ++pos) {
        int rem_pos = len - pos - 1;
        int start = (pos == 0) ? 1 : 0;
        for (int d = start; d <= 9; ++d) {
            int rem_sum = SUM - sum_so_far - d;
            if (rem_sum < 0) break;
            if (rem_sum > 9 * rem_pos) continue;

            uint64_t ways = dp[rem_pos][rem_sum];
            if (k > ways) {
                k -= ways;
            } else {
                res = res * 10 + (seq_t)d;
                sum_so_far += d;
                break;
            }
        }
    }
    return res;
}
