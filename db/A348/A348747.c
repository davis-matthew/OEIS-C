/*
 * A348747  Fully multiplicative with a(2) = 1, a(3) = 2, a(5) = 3,
 *          a(A002144(1+n)) = A002144(n) and a(A002145(1+n)) = a(A002145(1+n))
 *          for all n >= 1, where A002144 and A002145 give the primes of the
 *          form 4k+1 and 4k+3 respectively.
 * https://oeis.org/A348747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A348747
 *
 *  a(n) is fully multiplicative with the following prime values:
 *      a(2) = 1
 *      a(3) = 2
 *      a(5) = 3
 *      for a prime p > 5:
 *          if p ≡ 1 (mod 4)  →  a(p) = the previous prime ≡ 1 (mod 4)
 *          if p ≡ 3 (mod 4)  →  a(p) = p   (except the case p = 3 already handled)
 *  For composite n, a(n) = ∏ a(p)^e  where n = ∏ p^e.
 *
 *  This implementation factors n using simple trial division (sufficient for n ≤ 10 000
 *  as required).  The helper functions are fully internal and rely only on the
 *  standard C headers.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for small values (≤ 10 000).  */
static bool is_prime_simple(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Value of a(p) for a prime p.  */
static seq_t prime_value(uint64_t p)
{
    /* Handle the explicitly given small primes first. */
    if (p == 2) return (seq_t)1;
    if (p == 3) return (seq_t)2;
    if (p == 5) return (seq_t)3;

    if (p % 4 == 3)
        return (seq_t)p;        /* primes of the form 4k+3 (except 3) */

    /* p % 4 == 1 and p > 5: return the previous 4k+1 prime. */
    uint64_t prev = p - 4;
    while (prev >= 5) {
        if (is_prime_simple(prev))
            break;
        prev -= 4;
    }
    return (seq_t)prev;
}

/*  Fully multiplicative function a(n).  */
seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 1, and a(0) is undefined but harmless */
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Factor out powers of 2. */
    while ((m & 1ULL) == 0) {
        result *= prime_value(2);
        m >>= 1;
    }

    /* Factor out powers of 3. */
    while (m % 3 == 0) {
        result *= prime_value(3);
        m /= 3;
    }

    /* Factor out powers of 5. */
    while (m % 5 == 0) {
        result *= prime_value(5);
        m /= 5;
    }

    /* Trial division by odd numbers starting from 7. */
    for (uint64_t p = 7; p * p <= m; p += 2) {
        while (m % p == 0) {
            result *= prime_value(p);
            m /= p;
        }
    }

    /* If any prime factor remains, it is larger than sqrt(original n). */
    if (m > 1)
        result *= prime_value(m);

    return result;
}
