/*
 * A392059  a(n) is the product of all p^k such that p is a ramified or inert
 *          prime in the Gaussian integers and k is the largest such k
 *          satisfying p^k <= n.
 * https://oeis.org/A392059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = product over all primes p such that p == 2 or p ≡ 3 (mod
 *            4) of the largest power p^k with p^k ≤ n. In other words, a(n) =
 *            ∏_{p ∈ {2}∪{primes ≡ 3 mod 4 ≤ n}} p^⌊log_p n⌋
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: sequence starts at n = 1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0, safeguard */
    seq_t result = 1;

    /* simple sieve up to n: store primes in an array */
    /* maximum number of primes ≤ 10000 is 1229, safe for static buffer */
    uint64_t primes[1229];
    size_t  primes_cnt = 0;
    for (uint64_t i = 2; i <= n; ++i) {
        bool is_prime = true;
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint64_t p = primes[j];
            if (p * p > i) break;
            if (i % p == 0) { is_prime = false; break; }
        }
        if (is_prime) primes[primes_cnt++] = i;
    }

    /* iterate primes and multiply the largest power ≤ n */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p == 2 || (p & 3) == 3) {          /* p==2 or p≡3 (mod 4) */
            seq_t pow = 1;
            seq_t pseq = (seq_t)p;
            while (true) {
                seq_t next = pow * pseq;
                if (next > (seq_t)n) break;
                pow = next;
            }
            result *= pow;
        }
    }

    return result;
}
