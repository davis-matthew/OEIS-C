/*
 * A353509  a(n) = A353379(n) - A001222(n).
 * https://oeis.org/A353509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A353379(n) - A001222(n) where A353379(n) =
 *            A258851(A181819(n)) (primepi‑based arithmetic derivative applied
 *            to the prime shadow) and A001222(n) = sum of prime indices with
 *            multiplicity of n. The prime shadow of n is obtained by
 *            replacing each exponent e_i in the prime factorisation of n
 *            by...
 * Range:     u8 n <= 9984, u16 n <= 9984, u32 n <= 9984, u64 n <= 9984,
 *            u128 n <= 9984, big any n
 * Helpers:   seq_multiply
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes a static list of the first 200 primes,
   which is sufficient for all n up to at least 10 000.  It performs trial
   division using that list to factor numbers and compute the required values.
   All arithmetic uses the seq_t type provided by the build system. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_multiply.h"

static const int primes[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,
    179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,
    283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,
    419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,
    547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659,
    661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,
    811,821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,
    947,953,967,971,977,983,991,997,1009,1013,1019,1021,1031,1033,1039,1049,1051,1061,
    1063,1069,1087,1091,1093,1097,1103,1109,1117,1123,1129,1151,1153,1163,1171,1181,1187,
    1193,1201,1213,1217,1223
};
static const size_t PRIMES_COUNT = sizeof(primes) / sizeof(primes[0]);

/* Prime index table: prime_index[p] = pi(p), 1‑based.  Size is enough for the largest prime in the table. */
static unsigned short prime_index[1225] = {0};
static int initialized = 0;   /* non‑static for C++ and to avoid multiple initialisation pitfalls */

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    /* Initialise the prime index table once. */
    if (!initialized) {
        for (size_t i = 0; i < PRIMES_COUNT; ++i) {
            prime_index[primes[i]] = (unsigned short)(i + 1);
        }
        initialized = 1;
    }

    /* Step 1: compute the prime shadow S of n. */
    uint64_t tn = (uint64_t)n;
    seq_t S = (seq_t)1;
    for (size_t pi = 0; pi < PRIMES_COUNT && (uint64_t)primes[pi] * (uint64_t)primes[pi] <= tn; ++pi) {
        uint32_t e = 0;
        uint64_t p = (uint64_t)primes[pi];
        while (tn % p == 0) {
            ++e;
            tn /= p;
        }
        if (e > 0) {
            seq_t theta = (seq_t)primes[e - 1];   /* e‑th prime (1‑based) */
            S = seq_multiply(S, theta);
        }
    }
    if (tn > 1) {               /* remaining prime factor exponent 1 */
        seq_t theta = (seq_t)primes[0];  /* 1‑st prime = 2 */
        S = seq_multiply(S, theta);
    }

    /* Step 2: factor S to obtain D = A258851(S) and sum_pi = A001222(S). */
    uint64_t ts = (uint64_t)S;    /* S is small; fits in 64‑bit */
    seq_t D = (seq_t)0;
    seq_t sum_pi = (seq_t)0;

    for (size_t qi = 0; qi < PRIMES_COUNT && (uint64_t)primes[qi] * (uint64_t)primes[qi] <= ts; ++qi) {
        uint32_t f = 0;
        uint64_t q = (uint64_t)primes[qi];
        while (ts % q == 0) {
            ++f;
            ts /= q;
        }
        if (f > 0) {
            unsigned short pi_q = prime_index[(unsigned int)q];
            sum_pi += (seq_t)f * pi_q;
            D += (seq_t)f * pi_q * (S / (seq_t)q);
        }
    }
    if (ts > 1) {  /* remaining prime factor of S */
        unsigned short pi_ts = prime_index[(unsigned int)ts];
        sum_pi += pi_ts;                     /* exponent f = 1 */
        D += pi_ts * (S / (seq_t)ts);
    }

    return D - sum_pi;
}
