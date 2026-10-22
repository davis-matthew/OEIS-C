/*
 * A360130  a(n) = 1 if A003961(n) is a triangular number, otherwise 0, where
 *          A003961 is fully multiplicative with a(p) = nextprime(p).
 * https://oeis.org/A360130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A003961(n) is a triangular number, otherwise 0.
 *            A003961 is fully multiplicative with a(p) = nextprime(p)
 * Range:     u8 n <= 4, u16 n <= 69, u32 n <= 9866, u64 n <= 9866,
 *            u128 n <= 9866, big any n
 * Helpers:   _smallest_prime_factor, isqrt_seq, next_prime, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helpers used are supplied by the build system.  Include the
   corresponding headers. */
#include "../utils/_smallest_prime_factor.h"
#include "../utils/next_prime.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* n is at least 1; factorise n using the given utilities. */
    uint64_t tmp = (uint64_t)n;
    seq_t prod = 1;                       /* A003961(n) starts at 1 */

    while (tmp > 1) {
        uint64_t p = _smallest_prime_factor(tmp);   /* prime divisor p */
        idx_t e = 0;
        while (tmp % p == 0) {                       /* exponent of p */
            tmp /= p;
            ++e;
        }

        uint64_t np = next_prime(p);                 /* next prime after p */
        seq_t term = pow_uint(np, e);                /* np^e as seq_t */
        prod = seq_mul(prod, term);                  /* multiply into product */
    }

    /* Test whether prod is a triangular number.
       A triangular number T satisfies 8T+1 is a perfect square. */
    seq_t d  = seq_mul(prod, 8);        /* 8 * prod */
    d = d + (seq_t)1;                   /* + 1 */
    seq_t sqrt_d = isqrt_seq(d);
    seq_t sq = seq_mul(sqrt_d, sqrt_d);

    return (sq == d) ? 1 : 0;
}
