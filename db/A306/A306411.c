/*
 * A306411  a(n) = phi(n^6) = n^5*phi(n).
 * https://oeis.org/A306411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^5 * φ(n)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1626,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static seq_t phi_uint64(uint64_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t result = n;
    uint64_t m = n;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            result -= result / (seq_t)p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;
    seq_t n3 = n2 * tn;
    seq_t n4 = n3 * tn;
    seq_t n5 = n4 * tn;
    seq_t phi_val = phi_uint64((uint64_t)n);
    return n5 * phi_val;
}
