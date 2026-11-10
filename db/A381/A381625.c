/*
 * A381625  a(n) is the number of occurrences of n in A381624.
 * https://oeis.org/A381625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of m >= 0 with A381624(m) = n, i.e. the number of
 *            ways to write n = Sum_{k>=0} t_k*(2^(k+1)-1) with digits t_k in
 *            {0,1,2} (counted by pruned recursion)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 103 terms through n = 7570 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A381625_rec(idx_t n, int k, const unsigned __int128 *mx) {
    if (k < 0) return (seq_t)(n == 0 ? 1 : 0);
    if ((unsigned __int128)n > mx[k]) return (seq_t)0;
    idx_t w = (((idx_t)1) << (k + 1)) - 1;
    seq_t r = 0;
    for (int t = 0; t <= 2; t++) {
        unsigned __int128 tw = (unsigned __int128)w * t;
        if (tw > (unsigned __int128)n) break;
        r += A381625_rec(n - (idx_t)tw, k - 1, mx);
    }
    return r;
}

seq_t a(idx_t n) {
    unsigned __int128 mx[64];
    unsigned __int128 s = 0;
    for (int k = 0; k < 63; k++) {
        unsigned __int128 w = (((unsigned __int128)1) << (k + 1)) - 1;
        s += 2 * w;
        mx[k] = s;
    }
    int top = 0;
    while (top < 62 && ((((idx_t)1) << (top + 2)) - 1) <= n) top++;
    return A381625_rec(n, top, mx);
}
