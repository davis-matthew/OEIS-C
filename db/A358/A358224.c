/*
 * A358224  Parity of A328386(n), where A328386(n) = A276086(n) mod n, and
 *          A276086 is the primorial base exp-function.
 * https://oeis.org/A358224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000035(A328386(n))
 * Range:     u8 n <= 784, u16 n <= 9594, u32 n <= 9594, u64 n <= 9594,
 *            u128 n <= 9594, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the parity of A328386(n) where */
/* A328386(n) = A276086(n) mod n and A276086(n) = Π p_i^d_i   */
/* with n = Σ d_i * P_{i-1} , 0 ≤ d_i < p_i, P_k = ∏_{j=1..k} p_j. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* first 100 primes – sufficient for any uint64_t index */
static const idx_t _primes[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,
    59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,
    137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,
    227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,
    313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,
    419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,
    509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,
    617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,719,
    727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,
    829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941
};

static const size_t _prime_cnt = sizeof(_primes)/sizeof(_primes[0]);

/* Computes base^exp mod mod for any seq_t.  */
static inline seq_t _pow_mod(seq_t base, idx_t exp, seq_t mod)
{
    if (mod == 0) return 0;          /* should not happen for n>=1 */
    seq_t result = ((seq_t)1) % mod;
    seq_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (result * b) % mod;
        exp >>= 1;
        if (exp)
            b = (b * b) % mod;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not defined for offset 0 */

    /* Build primorials up to the largest ≤ n */
    idx_t primorials[64];
    size_t k = 0;                    /* last valid index in primorials */
    primorials[0] = 1;               /* P0 = 1 */

    while (k < _prime_cnt) {
        __uint128_t prod = (__uint128_t)primorials[k] * _primes[k];
        if (prod > n) break;
        primorials[++k] = (idx_t)prod;
    }
    /* k = index of largest primorial <= n */
    size_t maxi = k + 1;             /* number of digit positions to process */

    seq_t prod = ((seq_t)1) % (seq_t)n;
    idx_t rem = n;

    for (size_t ii = maxi; ii > 0; --ii) {
        idx_t weight = primorials[ii - 1];                 /* P_{ii-1} */
        idx_t digit = rem / weight;
        rem -= digit * weight;
        if (digit) {
            seq_t pow = _pow_mod(_primes[ii - 1], digit, (seq_t)n);
            prod = (prod * pow) % (seq_t)n;
        }
    }

    return prod & 1;   /* parity of A328386(n) */
}
