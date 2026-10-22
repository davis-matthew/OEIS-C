/*
 * A349754  a(n) = phi(A003961(n)) - 2*phi(n), where A003961 is fully
 *          multiplicative with a(p) = nextprime(p), and phi is Euler totient
 *          function.
 * https://oeis.org/A349754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = phi(A003961(n)) - 2*phi(n) where A003961 is
 *            multiplicative with a(p)=nextprime(p)
 * Range:     i8 n <= 31, i16 n <= 1023, i32 n <= 9468, i64 n <= 9468,
 *            i128 n <= 9468, big any n
 * Helpers:   factor_u64, next_prime, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    /* Factor n */
    factor_u64(N, primes, exps, &cnt);

    seq_t phi_n = 1;
    seq_t phi_A = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* phi(n) part: (p-1)*p^(e-1) */
        seq_t term_n = (seq_t)(p - 1) * pow_seq((seq_t)p, (int)(e - 1));
        phi_n *= term_n;

        /* phi(A003961(n)) part: (nextprime(p)-1)*nextprime(p)^(e-1) */
        uint64_t q = next_prime(p);
        seq_t term_A = (seq_t)(q - 1) * pow_seq((seq_t)q, (int)(e - 1));
        phi_A *= term_A;
    }

    /* n = 1 case: both products remain 1, giving a(1) = 1 - 2*1 = -1 */
    seq_t result = phi_A - (seq_t)2 * phi_n;
    return result;
}
