/*
 * A364292  Numbers k such that A163511(k) <= k.
 * https://oeis.org/A364292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A163511(k) <= k; found by a linear
 *            search, with A163511 computed from the binary expansion of k
 *            (early exit once the product exceeds k)
 * Range:     u8 n <= 53, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned A364292_pr[64] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,
    101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,
    193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,
    293,307,311
};

/* true if A163511(k) <= k */
static int A364292_ok(uint64_t k)
{
    uint64_t res = 1;
    unsigned prev = 0, pos = 0, i = 0;
    uint64_t t = k;
    while (t) {
        pos++;
        if (t & 1) {
            unsigned e = pos - prev - 1;
            uint64_t b = A364292_pr[i];
            while (e--) {
                if (res > k / b) return 0;
                res *= b;
            }
            i++;
            prev = pos;
        }
        t >>= 1;
    }
    {
        uint64_t b = A364292_pr[i - 1];
        if (res > k / b) return 0;
        res *= b;
    }
    return res <= k;
}

seq_t a(idx_t n)
{
    static uint64_t last_n = 0, last_k = 0;
    uint64_t cnt, k;
    if (last_n != 0 && last_n <= (uint64_t)n) {
        cnt = last_n;
        k = last_k;
    } else {
        cnt = 0;
        k = 0;
    }
    while (cnt < (uint64_t)n) {
        k++;
        if (A364292_ok(k)) cnt++;
    }
    last_n = cnt;
    last_k = k;
    return (seq_t)k;
}
