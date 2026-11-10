/*
 * A268819  Column 32769 of array A269158: a(n) = F(n,65537), function F as
 *          defined in A269158.
 * https://oeis.org/A268819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = F(n,65537) with F(p,q)=0 for p<=1; F(2p,q)=F(p,q) XOR (q
 *            XOR (q>>1)); F(2p+1,q)=F(q mod (2p+1), 2p+1) XOR ((2p+1) AND q)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 87 terms through n = 8162 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t p = (uint64_t)n;
    uint64_t q = 65537ULL;
    uint64_t acc = 0;
    while (p > 1) {
        if (p & 1ULL) {
            acc ^= (p & q);
            uint64_t r = q % p;
            q = p;
            p = r;
        } else {
            acc ^= (q ^ (q >> 1));
            p >>= 1;
        }
    }
    return (seq_t)acc;
}
