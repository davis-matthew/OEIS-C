/*
 * A245777  Denominator of (n / tau(n) - sigma(n) / n).
 * https://oeis.org/A245777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (n * tau(n)) / gcd(n^2 - sigma(n) * tau(n), n * tau(n))
 * Range:     u8 n <= 15, u16 n <= 1571, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau(n) is the divisor count and sigma(n) the sum of divisors. */

#include <stdint.h>
#include <stddef.h>

/* A list of small primes big enough for n up to many thousands. */
static const uint32_t small_primes[] = {
    2,3,5,7,11,13,17,19,23,29,
    31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,
    127,131,137,139,149,151,157,163,167,173,
    179,181,191,193,197,199,211,223,227,229,
    233,239,241,251,257,263,269,271,277,281,
    283,293,307,311,313,317,331,337,347,349,
    353,359,367,373,379,383,389,397,401,409,
    419,421,431,433,439,443,449,457,461,463,
    467,479,487,491,499,503,509,521,523,541,
    547,557,563,569,571,577,587,593,599,601,
    607,613,617,619,631,641,643,647,653,659,
    661,673,677,683,691,701,709,719,727,733,
    739,743,751,757,761,769,773,787,797,809,
    811,821,823,827,829,839,853,857,859,863,
    877,881,883,887,907,911,919,929,937,941,
    947,953,967,971,977,983,991,997
};
static const size_t small_primes_cnt = sizeof(small_primes) / sizeof(small_primes[0]);

/* Euclidean algorithm for seq_t. */
static inline seq_t _seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Compute tau(n) and sigma(n) by trial division on the small prime list. */
static void _tau_sigma(uint64_t n, seq_t *tau, seq_t *sigma)
{
    seq_t tau_val = 1;
    seq_t sigma_val = 1;
    uint64_t tmp = n;

    for (size_t i = 0; i < small_primes_cnt &&
         (uint64_t)small_primes[i] * (uint64_t)small_primes[i] <= tmp; ++i) {
        uint32_t p = small_primes[i];
        if (tmp % p == 0) {
            unsigned exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            tau_val *= (seq_t)(exp + 1);

            /* term = (p^(exp+1) - 1) / (p - 1) */
            seq_t p_pow = 1;
            for (unsigned k = 0; k < exp + 1; ++k) {
                p_pow *= (seq_t)p;
            }
            seq_t term = (p_pow - 1) / ((seq_t)p - 1);
            sigma_val *= term;
        }
    }

    if (tmp > 1) {          /* remaining prime factor */
        tau_val *= (seq_t)2;
        seq_t term = (seq_t)tmp + 1; /* (p^2-1)/(p-1) = p+1 */
        sigma_val *= term;
    }

    *tau = tau_val;
    *sigma = sigma_val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out-of-range guard */

    seq_t nn = (seq_t)n;
    seq_t tau, sigma;
    _tau_sigma((uint64_t)n, &tau, &sigma);

    seq_t a_val = nn * nn;
    seq_t b_val = sigma * tau;
    seq_t num = (a_val >= b_val) ? (a_val - b_val) : (b_val - a_val);
    seq_t den = nn * tau;

    if (num == 0) return (seq_t)1; /* zero difference -> denominator 1 */

    seq_t g = _seq_gcd(num, den);
    den /= g;
    return den;
}
