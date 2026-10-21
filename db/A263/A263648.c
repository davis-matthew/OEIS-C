/*
 * A263648  a(1)=4, a(2)=9: a(n) is the smallest semiprime not yet appearing
 *          in the sequence which is coprime to a(n-1) and not coprime to
 *          a(n-2).
 * https://oeis.org/A263648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * prime((n+1)/2) for odd n ≥ 15 a(n) = 3 * prime(n/2 -
 *            3) for even n ≥ 16 with the first 14 initial terms listed
 *            explicitly
 * Range:     u8 n <= 53, u16 n <= 4907, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Initial terms a(1) … a(14) */
    static const seq_t base[14] = {
        (seq_t)4,  (seq_t)9,  (seq_t)10, (seq_t)21, (seq_t)22,
        (seq_t)15, (seq_t)14, (seq_t)25, (seq_t)6,  (seq_t)35,
        (seq_t)26, (seq_t)49, (seq_t)34, (seq_t)77
    };

    if (n <= 14)          /* handle the prefix */
        return base[(size_t)(n - 1)];

    if (n & 1U) {            /* odd n */
        idx_t k = (n + 1) / 2;
        uint32_t p = get_prime(k);
        return (seq_t)2 * (seq_t)p;
    } else {                 /* even n */
        idx_t k = n / 2 - 3;
        uint32_t p = get_prime(k);
        return (seq_t)3 * (seq_t)p;
    }
}
