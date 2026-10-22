/*
 * A393064  a(n) = 1 if A020639(n') = A053669(n) and A053669(n') = A020639(n),
 *          otherwise 0, where n' stands for arithmetic derivative, A020639
 *          returns the least prime factor of its argument, and A053669 is the
 *          least prime not dividing its argument.
 * https://oeis.org/A393064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if A020639(n') = A053669(n) and A053669(n') =
 *            A020639(n), else 0 where n' is the arithmetic derivative of n,
 *            A020639 is the least prime factor, A053669 is the least prime
 *            not dividing its argument
 * Range:     u8 n <= 9762, u16 n <= 9762, u32 n <= 9762, u64 n <= 9762,
 *            u128 n <= 9762, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* First 100 primes up to 541 – sufficient for n up to 10 000. */
static const uint64_t PRIMES[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,
    67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,
    149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,
    233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,
    331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,
    431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,
    523,541
};
static const size_t PRIMESCNT = sizeof(PRIMES) / sizeof(PRIMES[0]);

/* Least prime factor of n.
   Convention: A020639(0) = 2, A020639(1) = 1. */
static uint64_t lpf_uint64(uint64_t n)
{
    if (n == 0) return 2;
    if (n == 1) return 1;
    if (n % 2 == 0) return 2;
    for (size_t i = 1; i < PRIMESCNT; ++i) {
        uint64_t p = PRIMES[i];
        if (p * p > n) break;
        if (n % p == 0) return p;
    }
    return n; /* n is prime */
}

/* Least prime not dividing n.
   Convention: A053669(0) = 1, A053669(1) = 2. */
static uint64_t lpn_uint64(uint64_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;
    for (size_t i = 0; i < PRIMESCNT; ++i) {
        uint64_t p = PRIMES[i];
        if (n % p != 0) return p;
    }
    /* In the unlikely event that all primes in PRIMES divide n,
       extend the search (should never happen for n ≤ 2^64). */
    uint64_t p = PRIMES[PRIMESCNT - 1] + 2;
    while (1) {
        /* simple primality test */
        int is_prime = 1;
        for (uint64_t d = 3; d * d <= p; d += 2) {
            if (p % d == 0) { is_prime = 0; break; }
        }
        if (!is_prime) { p += 2; continue; }
        if (n % p != 0) return p;
        p += 2;
    }
}

/* Arithmetic derivative of n.
   For n ≤ 1 it is defined as 0. */
static uint64_t arithmetic_derivative_uint64(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t original = n;
    uint64_t d = 0;
    for (size_t i = 0; i < PRIMESCNT; ++i) {
        uint64_t p = PRIMES[i];
        if (p * p > n) break;
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            d += exp * (original / p);
        }
    }
    if (n > 1) { /* remaining prime factor */
        d += 1 * (original / n);
    }
    return d;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t d   = arithmetic_derivative_uint64(nn);

    uint64_t lp_n  = lpf_uint64(nn);
    uint64_t lp_d  = lpf_uint64(d);
    uint64_t lpn_n = lpn_uint64(nn);
    uint64_t lpn_d = lpn_uint64(d);

    return (seq_t)(((lp_n == lpn_d) && (lp_d == lpn_n)) ? 1 : 0);
}
