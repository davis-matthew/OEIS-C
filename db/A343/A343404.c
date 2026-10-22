/*
 * A343404  For any number n with representation (d_w, ..., d_1) in primorial
 *          base, a(n) is the least number m such that m mod prime(k) = d_k
 *          for k = 1..w (where prime(k) denotes the k-th prime number).
 * https://oeis.org/A343404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 210, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A343404:
 * For the primorial–base expansion of n = Σ d_k * P_{k-1}, where
 * P_k = p_1 * p_2 * … * p_k and p_k is the k‑th prime,
 * the result is the least non‑negative integer m satisfying
 * m ≡ d_k (mod p_k) for all k.
 * The implementation builds the expansion digit by digit and
 * then solves the system of congruences using a simple
 * incremental Chinese remainder routine that is fast enough
 * for n up to at least 10 000.
 */
#include <stddef.h>
#include <stdbool.h>

/* first 25 primes, sufficient for n up to at least 10 000 */
static const unsigned int _PRIMES[25] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */

    seq_t m   = 0;                  /* current CRT solution */
    seq_t mod = 1;                  /* modulus of the current solution */
    idx_t temp = n;                 /* remaining part of n to decompose */

    size_t k = 0;                   /* index into _PRIMES */
    while (temp > 0)
    {
        unsigned int p = _PRIMES[k++];         /* next prime */
        seq_t digit = (seq_t)(temp % (idx_t)p);/* digit in this position */

        /* find the smallest t in [0,p-1] such that
           (m + t*mod) ≡ digit (mod p)                   */
        unsigned int t;
        seq_t candidate;
        for (t = 0; t < p; ++t)
        {
            candidate = m + (seq_t)t * mod;
            if ((candidate % (seq_t)p) == digit)
            {
                m = candidate;
                break;
            }
        }

        /* update modulus to product of processed primes */
        mod = mod * (seq_t)p;
        temp = temp / (idx_t)p;  /* remove the factor for next digit */
    }
    return m;                      /* least non‑negative solution */
}
