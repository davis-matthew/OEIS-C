/*
 * A364287  Numbers k such that A163511(k) < k.
 * https://oeis.org/A364287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A163511(k) < k, where A163511(k) = p(r)
 *            * Product_{i=1..r} p(i)^(g_i), r = A000120(k), g_i = gap before
 *            i-th set bit; found by a linear search
 * Range:     u8 n <= 46, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A364287_less(uint64_t k)
{
    static const uint64_t pr[64] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,
        101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,
        193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,
        293,307,311
    };
    int r = 0;
    uint64_t t = k;
    while (t) { r += (int)(t & 1); t >>= 1; }
    uint64_t res = pr[r - 1];
    if (res >= k) return false;
    int prev = 0, bitpos = 1, i = 0;
    t = k;
    while (t) {
        if (t & 1) {
            int e = bitpos - prev - 1;
            uint64_t b = pr[i];
            for (int j = 0; j < e; j++) {
                if (res > (k - 1) / b) return false; /* res*b >= k */
                res *= b;
            }
            prev = bitpos;
            i++;
        }
        bitpos++;
        t >>= 1;
    }
    return res < k;
}

seq_t a(idx_t n)
{
    static uint64_t last_k = 0;
    static uint64_t last_n = 0;
    uint64_t k, cnt;
    if (last_n != 0 && last_n <= (uint64_t)n) {
        k = last_k;
        cnt = last_n;
    } else {
        k = 0;
        cnt = 0;
    }
    while (cnt < (uint64_t)n) {
        k++;
        if (A364287_less(k)) cnt++;
    }
    last_k = k;
    last_n = cnt;
    return (seq_t)k;
}
