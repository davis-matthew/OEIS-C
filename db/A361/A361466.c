/*
 * A361466  a(n) = 1 if A017665(A003961(n)) is a power of 2, otherwise 0. Here
 *          A017665 is the numerator of the sum of the reciprocals of the
 *          divisors of n, and A003961 is fully multiplicative with a(p) =
 *          nextprime(p).
 * https://oeis.org/A361466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A017665(A003961(n)) is a power of 2, else 0
 * Range:     u8 n <= 31, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_power_of_two, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>

#include "../utils/next_prime.h"
#include "../utils/gcd.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    if (n < 1u)            // position 0 not used, return 0
        return (seq_t)0;

    /* factor n: primes[] holds distinct primes, exps[] holds exponents */
    uint64_t primes[64];
    int exps[64];
    int cnt = 0;
    uint64_t nn = (uint64_t)n;
    /* factor 2 */
    int e = 0;
    while ((nn & 1u) == 0u) {
        e++;
        nn >>= 1u;
    }
    if (e) {
        primes[cnt] = 2u;
        exps[cnt] = e;
        cnt++;
    }
    /* odd factors */
    for (uint64_t p = 3; p * p <= nn; p += 2) {
        if (nn % p) continue;
        e = 0;
        while (nn % p == 0) {
            e++;
            nn /= p;
        }
        primes[cnt] = p;
        exps[cnt] = e;
        cnt++;
    }
    if (nn > 1) {           /* remaining prime */
        primes[cnt] = nn;
        exps[cnt] = 1;
        cnt++;
    }

    /* compute m = A003961(n) = product nextprime(p)^e and
       sigma = σ(m) = product (q^(e+1)-1)/(q-1)   with q = nextprime(p)  */
    unsigned __int128 m = 1;
    unsigned __int128 sigma = 1;
    for (int i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int    e  = exps[i];
        uint64_t q = next_prime(p);          /* q = next prime after p */

        /* q^e */
        unsigned __int128 qpow = 1;
        for (int k = 0; k < e; ++k) qpow *= q;

        m *= qpow;

        /* sigma_piece = (q^(e+1)-1)/(q-1) */
        unsigned __int128 qpow_next = qpow * q;
        unsigned __int128 sigma_piece = (qpow_next - 1u) / (q - 1u);

        sigma *= sigma_piece;
    }

    /* Convert to seq_t (guaranteed to fit on inputs < 10000) */
    seq_t m_seq   = (seq_t)(uint64_t)m;
    seq_t sigma_seq = (seq_t)(uint64_t)sigma;

    seq_t g = gcd(m_seq, sigma_seq);
    seq_t num = sigma_seq / g;

    return (seq_t)(is_power_of_two(num) ? 1 : 0);
}
