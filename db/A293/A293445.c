/*
 * A293445  A multiplicative encoding (base-2 compressed) for the exponents of
 *          3 obtained when using Shevelev's algorithm for computing A053446.
 * https://oeis.org/A293445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let m = A001651(n) = floor((3n-1)/2). Start with x = 1. Repeat:
 *            choose d in {1,2} with 3 | x+d*m, let e = 3-adic valuation of
 *            x+d*m, set x = (x+d*m)/3^e, and multiply the result by
 *            A019565(e). Stop when x = 1. Then A048675(a(n)) = A053446(n)
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 28, u64 n <= 52, u128 n <= 99,
 *            big any n
 * Validated: standard, pass, 71 terms through n = 1397 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A293445_pr[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

/* squarefree number whose primes are prime(k+1) for set bits k of e */
static seq_t A293445_sqf(uint64_t e)
{
    seq_t r = 1;
    unsigned bit = 0;
    while (e != 0) {
        if (e & 1ULL) r *= (seq_t)A293445_pr[bit];
        e >>= 1;
        ++bit;
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t m = (3ULL * (uint64_t)n - 1ULL) / 2ULL;
    uint64_t x = 1;
    seq_t res = 1;
    do {
        uint64_t v = x + m;
        if (v % 3 != 0) v = x + 2 * m;
        uint64_t e = 0;
        while (v % 3 == 0) {
            v /= 3;
            ++e;
        }
        x = v;
        res *= A293445_sqf(e);
    } while (x != 1);
    return res;
}
