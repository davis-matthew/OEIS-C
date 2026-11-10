/*
 * A341694  Square array T(n, k) read by antidiagonals upwards, n, k > 0: T(n,
 *          k) = A227736(n, k) for k = 1..A005811(n), and T(n, k) = T(n, k -
 *          A005811(n)) + ... + T(n, k-1) for k > A005811(n).
 * https://oeis.org/A341694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   T(n,k) = k-th run length of n from the LSB for k <= r =
 *            A005811(n); T(n,k) = T(n,k-r)+...+T(n,k-1) for k > r; a(m) =
 *            T(n,k) read by antidiagonals upwards (n = d-pos+1, k = pos)
 * Range:     u8 n <= 118, u16 n <= 320, u32 n <= 941, u64 n <= 3230,
 *            u128 n <= 11016, big any n
 * Validated: standard, pass, 94 terms through n = 9686 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t m)
{
    idx_t d = 1, rem = m;
    while (rem > d) { rem -= d; d++; }
    idx_t k = rem;
    idx_t n = d - rem + 1;

    idx_t runs[70];
    int r = 0;
    idx_t x = n;
    while (x) {
        idx_t bit = x & 1;
        idx_t c = 0;
        while (x && (x & 1) == bit) { c++; x >>= 1; }
        runs[r++] = c;
    }

    long long K = (long long)k;
    seq_t *t = mem_new(seq_t, (size_t)K + 1);
    seq_t window = 0;
    for (long long i = 1; i <= K; i++) {
        seq_t v;
        if (i <= r) {
            v = (seq_t)runs[i - 1];
        } else {
            v = window;
        }
        t[i] = v;
        window += v;
        if (i - r >= 1) window -= t[i - r];
    }
    seq_t res = t[K];
    mem_free(t);
    return res;
}
