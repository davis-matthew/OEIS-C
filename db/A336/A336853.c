/*
 * A336853  a(n) = A003961(n) - n, where A003961 is the prime shift towards
 *          larger primes.
 * https://oeis.org/A336853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003961(n) - n where A003961(n) replaces every prime
 *            factor p of n by the next prime after p (keeping the exponent)
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 9678, u64 n <= 9678,
 *            u128 n <= 9678, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: idx_t is an unsigned integer type (e.g. uint64_t).  seq_t
   is an unsigned integer type of arbitrary size.  The routine uses
   only integer arithmetic and the utilities listed in the problem statement. */

#include <stdint.h>
#include <stdbool.h>

/* --- minimal prime test for 64‑bit values --------------------------------- */
static bool is_prime_uint64(uint64_t v)
{
    if (v < 2)            return false;
    if (v == 2 || v == 3) return true;
    if ((v & 1) == 0)     return false;          /* even */
    for (uint64_t d = 3; d * d <= v; d += 2)
        if (v % d == 0) return false;
    return true;
}

/* --- next prime strictly greater than n ------------------------------ */
static uint64_t next_prime_gt(uint64_t n)
{
    if (n < 2) return 2;          /* next prime after 0 or 1 is 2 */
    uint64_t p = n + 1;
    if ((p & 1) == 0) if (p != 2) p++;          /* make odd if needed */
    while (!is_prime_uint64(p)) {
        p += 2;                     /* skip even numbers */
    }
    return p;
}

/* --- main sequence routine ---------------------------------------------- */
seq_t a(idx_t n)
{
    /* A003961(1) is defined as 1, so a(1)=1-1=0.  */
    if (n == 1) return 0;

    uint64_t temp = n;                /* factorization done on 64‑bit value */
    seq_t result = 1;

    /* ----- factor 2 separately ------------------------------------------ */
    if ((temp & 1) == 0) {
        uint64_t exp = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++exp;
        }
        uint64_t q = next_prime_gt(2);       /* 3 */
        seq_t pow = 1;
        for (uint64_t k = 0; k < exp; ++k)
            pow *= (seq_t)q;
        result *= pow;
    }

    /* ----- odd prime factors ------------------------------------------- */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            uint64_t q = next_prime_gt(p);
            seq_t pow = 1;
            for (uint64_t k = 0; k < exp; ++k)
                pow *= (seq_t)q;
            result *= pow;
        }
    }

    /* ----- remaining prime factor > sqrt(n) ----------------------------- */
    if (temp > 1) {
        uint64_t q = next_prime_gt(temp);
        result *= (seq_t)q;
    }

    /* Final answer: A003961(n) - n */
    return result - (seq_t)n;
}
