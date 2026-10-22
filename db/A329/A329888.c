/*
 * A329888  a(n) = A329900(A329602(n)); Heinz number of the even bisection
 *          (even-indexed parts) of the integer partition with Heinz number n.
 * https://oeis.org/A329888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of primes at even positions in the weakly *
 *            decreasing list (with repetition) of the prime factors of n. *
 *            (Indices 1‑based; e.g., for 108 = 3^3·2^2, the list is
 *            (3,3,3,2,2); * even‑indexed primes are (3,2), so a(108)=6.)
 * Range:     u8 n <= 9379, u16 n <= 9379, u32 n <= 9379, u64 n <= 9379,
 *            u128 n <= 9379, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases. */
    if (n <= 1u) return (seq_t)1;
    uint64_t temp = n;

    /* Store prime factors and their exponents in ascending order. */
    uint64_t primes[65];
    uint64_t exps[65];
    int pc = 0;

    /* Factor 2 separately. */
    if ((temp & 1u) == 0u) {
        uint64_t e = 0;
        while ((temp & 1u) == 0u) {
            temp >>= 1u;
            ++e;
        }
        primes[pc] = 2u;
        exps[pc] = e;
        ++pc;
    }

    /* Trial division by odd candidates. */
    for (uint64_t p = 3u; p <= temp / p; p += 2u) {
        if (temp % p == 0u) {
            uint64_t e = 0;
            while (temp % p == 0u) {
                temp /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc] = e;
            ++pc;
        }
    }

    /* If anything remains, it's a prime > sqrt(original n). */
    if (temp > 1u) {
        primes[pc] = temp;
        exps[pc] = 1u;
        ++pc;
    }

    /* Compute the product of primes at even positions in the
       descending flattened list. */
    uint64_t result = 1u;          /* fits in 64 bits because it divides n */
    uint64_t processed = 0u;       /* number of positions already filled */

    for (int i = pc - 1; i >= 0; --i) {   /* iterate in descending order */
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t start = processed + 1u;   /* first position of this block */
        uint64_t evens = (start & 1u) ? (e >> 1u)           /* odd start */
                                     : ((e + 1u) >> 1u);   /* even start */

        for (uint64_t k = 0; k < evens; ++k) {
            result *= p;            /* safe: result <= n and result * p <= n */
        }
        processed += e;
    }

    return (seq_t)result;
}
