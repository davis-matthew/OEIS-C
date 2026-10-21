/*
 * A294081  Number of partitions of n into three squares and two nonnegative
 *          7th powers.
 * https://oeis.org/A294081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered partitions of n into three squares
 *            and two nonnegative 7th powers
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation pre‑computes the number of unordered triples of squares up to n
   and counts the unordered pairs of 7th powers.  The function works for any
   index n >= 0.  The algorithm runs in O(sqrt(n)^3 + n^(1/7)^2) time, which is
   easily fast enough for n up to about 10 000.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer square root for 64‑bit unsigned values (binary search). */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid == 0 || mid <= n / mid) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Compute the number of unordered triples of squares summing to each value
   up to `max_n`.  The array `cnt` must be allocated with size max_n+1
   and be zero‑initialised. */
static void compute_three_squares(idx_t max_n, seq_t *cnt)
{
    idx_t lim = isqrt_u64(max_n);
    for (idx_t x = 0; x <= lim; ++x) {
        idx_t x2 = x * x;
        for (idx_t y = x; y <= lim; ++y) {
            idx_t y2 = y * y;
            idx_t sum2 = x2 + y2;
            if (sum2 > max_n) {
                break;
            }
            for (idx_t z = y; z <= lim; ++z) {
                idx_t total = sum2 + z * z;
                if (total > max_n) {
                    break;
                }
                cnt[total] += 1;
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Allocate and compute the table of triples of squares. */
    size_t sz = (size_t)(n + 1);
    seq_t *cnt = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!cnt) return 0;  /* allocation failure – return zero as sentinel */

    compute_three_squares(n, cnt);

    /* Build the list of 7th powers <= n.  0^7 is always included. */
    idx_t pow7[32];
    size_t pcnt = 0;
    for (idx_t i = 0; ; ++i) {
        idx_t v;
        if (i == 0) {
            v = 0;
        } else {
            v = 1;
            for (int k = 0; k < 7; ++k) {
                if (v > n / i) {  /* would overflow or exceed n */
                    v = n + 1;
                    break;
                }
                v *= i;
            }
        }
        if (v > n) {
            break;
        }
        pow7[pcnt++] = v;
        if (pcnt >= sizeof(pow7)/sizeof(pow7[0])) {
            break;  /* safety in case n is huge */
        }
    }

    /* Count partitions using unordered pairs of 7th powers. */
    seq_t ans = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        for (size_t j = i; j < pcnt; ++j) {
            idx_t sum7 = pow7[i] + pow7[j];
            if (sum7 > n) {
                break;
            }
            ans += cnt[(size_t)(n - sum7)];
        }
    }

    free(cnt);
    return ans;
}
