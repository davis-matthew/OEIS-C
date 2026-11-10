/*
 * A344535  For any number n with binary expansion Sum_{k = 1..m} 2^e_k (where
 *          0 <= e_1 < ... < e_m), a(n) = Product_{k = 1..m}
 *          prime(1+A025581(e_k))^2^A002262(e_k) (where prime(k) denotes the
 *          k-th prime number).
 * https://oeis.org/A344535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Product_{k} prime(1+A025581(e_k))^(2^A002262(e_k)), over
 *            the set bits e_k of n
 * Range:     u8 n <= 26, u16 n <= 126, u32 n <= 957, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 74 terms through n = 7606 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t a344535_nth_prime(uint64_t k)
{
    uint64_t p = 1;
    while (k > 0) {
        p++;
        bool_label:
        ;
        uint64_t d;
        int isp = 1;
        for (d = 2; d * d <= p; d++) {
            if (p % d == 0) { isp = 0; break; }
        }
        if (isp) k--;
    }
    return p;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    unsigned e = 0;
    while (n) {
        if (n & 1) {
            /* row r and column c of e in the triangle */
            uint64_t r = 0;
            while ((r + 1) * (r + 2) / 2 <= (uint64_t)e) r++;
            uint64_t c = (uint64_t)e - r * (r + 1) / 2;
            uint64_t pidx = (r - c) + 1;
            seq_t p = (seq_t)a344535_nth_prime(pidx);
            uint64_t i;
            for (i = 0; i < c; i++) p = p * p;
            result = result * p;
        }
        n >>= 1;
        e++;
    }
    return result;
}
