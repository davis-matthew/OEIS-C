/*
 * A364258  a(n) = A163511(n) - n.
 * https://oeis.org/A364258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A163511(n) - n, where A163511(0)=1 and, for n>0 with r =
 *            A000120(n) set bits at 1-based positions b_1<...<b_r, A163511(n)
 *            = p(r) * Product_{i=1..r} p(i)^(b_i - b_{i-1} - 1), b_0 = 0
 * Range:     i8 n <= 32, i16 n <= 258, i32 n <= 4110, i64 n <= 12288,
 *            i128 n <= 12288, big any n
 * Validated: standard, pass, 91 terms through n = 9019 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A364258_prime(unsigned k) /* k-th prime, k>=1 */
{
    uint64_t c = 1;
    unsigned cnt = 0;
    while (cnt < k) {
        c++;
        bool_dummy:
        ;
        uint64_t d;
        int ok = 1;
        for (d = 2; d * d <= c; d++) {
            if (c % d == 0) { ok = 0; break; }
        }
        if (ok) cnt++;
    }
    return c;
}

static seq_t A364258_A163511(idx_t n)
{
    if (n == 0) return (seq_t)1;
    unsigned pos[64];
    unsigned cnt = 0;
    unsigned bitpos = 1;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) pos[cnt++] = bitpos;
        bitpos++;
        tmp >>= 1;
    }
    seq_t result = (seq_t)A364258_prime(cnt);
    unsigned prev = 0;
    for (unsigned i = 0; i < cnt; i++) {
        unsigned ex = pos[i] - prev - 1;
        if (ex) {
            seq_t base = (seq_t)A364258_prime(i + 1);
            for (unsigned e = 0; e < ex; e++) result *= base;
        }
        prev = pos[i];
    }
    return result;
}

seq_t a(idx_t n)
{
    return A364258_A163511(n) - (seq_t)n;
}
