/*
 * A366245  The largest infinitary divisor of n that is a term of A366243.
 * https://oeis.org/A366245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} p^{f(e)} where f(e) = 2 if e has bit 1 set
 *            (i.e., e ≡ 2 or 3 (mod 4)), 0 otherwise. This follows from
 *            a(p^e) = p^A063695(e) with A063695(e) = e & 2. The function
 *            computes the largest infinitary divisor of n that is a term of
 *            A366243 (numbers whose prime exponents are all 2)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* calls the function with external linkage as required */
seq_t a(idx_t n)
{
    /* Handle n = 1 directly */
    if (n == 1) return 1;

    /* Working copy of n for factorisation */
    uint64_t temp = n;
    seq_t res = 1;

    /* Factor out the prime 2 separately */
    uint64_t e = 0;
    while ((temp & 1ULL) == 0) {
        e++;
        temp >>= 1;
    }
    if (e & 2ULL)      /* include 2^2 if e has bit 1 set */
        res *= (seq_t)4;   /* 2^2 = 4 */

    /* Factor odd primes */
    for (uint64_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            e = 0;
            while (temp % d == 0) {
                e++;
                temp /= d;
            }
            if (e & 2ULL) { /* include d^2 if e has bit 1 set */
                res *= (seq_t)d * (seq_t)d;
            }
        }
    }

    /* Remaining prime factor (if any) has exponent 1, which is not
       considered for a(n) because 1's binary representation does
       not contain the 2^1 bit. */
    return res;
}
