/*
 * A373585  a(n) = 1 if A373149(n) is even, otherwise 0, where A373149 is
 *          fully additive with a(2) = 1 and a(p) = prevprime(p) for odd
 *          primes p.
 * https://oeis.org/A373585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A059841(A373149(n))
 * Range:     u8 n <= 9030, u16 n <= 9030, u32 n <= 9030, u64 n <= 9030,
 *            u128 n <= 9030, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of range guard */

    idx_t temp = n;
    bool parity = false;                /* parity of Σ_{p≠3} e(p) */

    /* factor 2 */
    idx_t e = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        e++;
    }
    if (e & 1) parity ^= 1;

    /* odd prime factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            if (p != 3 && (e & 1)) parity ^= 1;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        if (temp != 3) parity ^= 1;
    }

    /* a(n) = 1 if A373149(n) is even (parity==0), else 0 */
    return (seq_t)(parity ? 0 : 1);
}
