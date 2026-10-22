/*
 * A375038  Irregular triangle read by rows T(n,k), n >= 2, k >= 1, in which
 *          row n lists the nonmiddle divisors of n.
 * https://oeis.org/A375038
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 1287, u16 n <= 6375, u32 n <= 6375, u64 n <= 6375,
 *            u128 n <= 6375, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A375038 – Flattened a triangle of non‑middle divisors.
 *
 * A(n) is the n‑th entry of the triangle when all rows are concatenated
 * in increasing order of n (rows start at n = 2).
 *
 * A row for a given integer m lists every divisor d of m such that
 * d is NOT in the half‑open interval [ sqrt(m/2) , sqrt(2m) ).
 * The divisors are listed in strictly increasing order.
 *
 * This implementation iterates over m = 2,3,… until the cumulative
 * count of printed divisors reaches the requested index.
 * For each row the list of valid divisors is built in two separate
 * buffers: low (the small divisors) and high (the complementary large
 * divisors).  The high buffer is reversed after collection to obtain
 * ascending order.
 *
 * The routine is structured to avoid floating point arithmetic – only
 * integer operations are used.  Intermediate squares are computed in
 * 128‑bit arithmetic to avoid overflow.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static inline idx_t _isqrt_u64(idx_t n)
{
    /* integer floor of the square root of n */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

static inline bool _is_excluded(idx_t d, idx_t m)
{
    /* Returns true if the divisor d lies in [sqrt(m/2), sqrt(2m)) */
    unsigned __int128 d2 = (unsigned __int128)d * d;
    return (2 * d2 >= m) && (d2 < 2 * (unsigned __int128)m);
}

#define MAX_DIV 256  /* sufficiently large for rows up to ~10k */

static void _collect_valid_divisors(idx_t m, idx_t *low, size_t *low_len,
                                    idx_t *high, size_t *high_len)
{
    idx_t sqrt_m = _isqrt_u64(m);
    size_t l = 0, h = 0;

    for (idx_t i = 1; i <= sqrt_m; ++i) {
        if (m % i == 0) {
            idx_t d = i;
            idx_t q = m / i;
            if (!_is_excluded(d, m)) {
                low[l++] = d;
            }
            if (q != d && !_is_excluded(q, m)) {
                high[h++] = q;
            }
        }
    }

    /* reverse high[] to obtain ascending order */
    if (h > 0) {
        size_t i = 0;
        size_t j = h - 1;
        while (i < j) {
            idx_t tmp = high[i];
            high[i] = high[j];
            high[j] = tmp;
            ++i;
            --j;
        }
    }

    *low_len = l;
    *high_len = h;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* indices below the offset are undefined */

    idx_t target = n - 2;          /* zero‑based index in the flattened list */
    idx_t cum = 0;                 /* cumulative count of divisors seen so far */

    for (idx_t m = 2; ; ++m) {
        /* Count the number of non‑middle divisors of m */
        idx_t sqrt_m = _isqrt_u64(m);
        idx_t cnt = 0;
        for (idx_t i = 1; i <= sqrt_m; ++i) {
            if (m % i == 0) {
                idx_t d = i;
                idx_t q = m / i;
                if (!_is_excluded(d, m)) ++cnt;
                if (q != d && !_is_excluded(q, m)) ++cnt;
            }
        }

        if (cum + cnt > target) {
            /* The desired divisor is in this row */
            idx_t offset = target - cum;
            idx_t low[MAX_DIV], high[MAX_DIV];
            size_t low_len = 0, high_len = 0;
            _collect_valid_divisors(m, low, &low_len, high, &high_len);

            if ((seq_t)offset < low_len) {
                return (seq_t)low[offset];
            } else {
                return (seq_t)high[offset - low_len];
            }
        }

        cum += cnt;
    }

    /* Control should never reach here */
    return (seq_t)0;
}
