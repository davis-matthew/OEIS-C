/*
 * A268719  Triangular table T(n>=0,k=0..n) = A003188(A006068(n) +
 *          A006068(k)), read by rows as A(0,0), A(1,0), A(1,1), A(2,0),
 *          A(2,1), A(2,2), ...
 * https://oeis.org/A268719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = A003188(A006068(n) + A006068(k)), with row r =
 *            A003056(m), column k = A002262(m) for linear index m = a(m)
 * Range:     u8 n <= 8256, u16 n <= 15050, u32 n <= 15050, u64 n <= 15050,
 *            u128 n <= 15050, big any n
 * Validated: standard, pass, 107 terms through n = 9817 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A268719_inv_gray(seq_t x)
{
    seq_t result = x;
    seq_t shifted = x;
    while (shifted > 0) {
        shifted >>= 1;
        result ^= shifted;
    }
    return result;
}

static seq_t A268719_gray(seq_t x)
{
    return x ^ (x >> 1);
}

seq_t a(idx_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 33;
    unsigned __int128 N = (unsigned __int128)n;
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= N)
            lo = mid;
        else
            hi = mid;
    }
    uint64_t r = lo;
    uint64_t k = (uint64_t)(N - (unsigned __int128)r * (r + 1) / 2);
    seq_t s = A268719_inv_gray((seq_t)r) + A268719_inv_gray((seq_t)k);
    return A268719_gray(s);
}
