/*
 * A316354  Triangle read by rows: T(1,1)=1, T(n,k) =
 *          T(n,k+1)+T(n-k,max(2*floor(k/2)-1,1)) and T(n,k) = 0 if k >
 *          A316355(n).
 * https://oeis.org/A316354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(1,1)=1; T(n,k) = T(n,k+1) + T(n-k, max(2*floor(k/2)-1,1)),
 *            T(n,k)=0 for k > A316355(n); a(n) is the triangle read by rows
 * Range:     u8 n <= 30, u16 n <= 84, u32 n <= 242, u64 n <= 710,
 *            u128 n <= 2047, big any n
 * Validated: standard, pass, 81 terms through n = 9814 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* row length: A316355(n), with 2k-2 once then 2k-1 (2k times) */
static idx_t A316354_len(idx_t n) {
    idx_t lo = 0, hi = ((idx_t)1 << 32);
    idx_t t = n + 1;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid <= (unsigned __int128)t) lo = mid + 1;
        else hi = mid;
    }
    idx_t k = lo - 1;
    idx_t start = k * k - 1;
    return (n == start) ? (2 * k - 2) : (2 * k - 1);
}

static idx_t A316354_m(idx_t k) {
    idx_t v = 2 * (k / 2);
    v = (v >= 1) ? v - 1 : 0;
    return v < 1 ? 1 : v;
}

seq_t a(idx_t n) {
    idx_t *off = (idx_t *)malloc((size_t)(n + 2) * sizeof(idx_t));
    idx_t row = 1, rem = n, total = 0;
    off[1] = 0;
    for (;;) {
        idx_t L = A316354_len(row);
        if (rem <= L) break;
        rem -= L;
        total += L;
        row++;
        off[row] = total;
    }
    idx_t col = rem;
    idx_t size = off[row] + A316354_len(row);
    seq_t *T = mem_new(seq_t, (size_t)size + 1);
    T[0] = 1;
    for (idx_t r = 2; r <= row; r++) {
        idx_t L = A316354_len(r);
        seq_t run = 0;
        for (idx_t k = L; k >= 1; k--) {
            run = run + T[off[r - k] + A316354_m(k) - 1];
            T[off[r] + k - 1] = run;
        }
    }
    seq_t res = T[off[row] + col - 1];
    mem_free(T);
    free(off);
    return res;
}
