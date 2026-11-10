/*
 * A276147  a(2n+1) = 2n+1, a(2n) = A053669(2n) * a(A111701(2n)).
 * https://oeis.org/A276147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n odd; a(n) = A053669(n) * a(A111701(n)) if n even
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 2310, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Validated: standard, pass, 102 terms through n = 2293 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A276147_isp(uint64_t q) {
    if (q < 2) return false;
    for (uint64_t d = 2; d * d <= q; d++)
        if (q % d == 0) return false;
    return true;
}

static uint64_t A276147_nextp(uint64_t p) {
    p++;
    while (!A276147_isp(p)) p++;
    return p;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t cur = (uint64_t)n;
    while (cur % 2 == 0) {
        /* A053669: smallest prime not dividing cur */
        uint64_t p = 2;
        while (cur % p == 0) p = A276147_nextp(p);
        result *= (seq_t)p;
        /* A111701: divide successively by 2,3,5,... */
        uint64_t m = cur;
        uint64_t q = 2;
        while (m % q == 0) {
            m /= q;
            if (m == 1) break;
            q = A276147_nextp(q);
        }
        cur = m;
    }
    result *= (seq_t)cur;
    return result;
}
