/*
 * A345993  Let m = A344005(n) = smallest m such that n divides m*(m+1); a(n)
 *          = gcd(n,m+1).
 * https://oeis.org/A345993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A345993: a(n) = gcd(n, m+1) where m is the smallest
   positive integer such that n divides m*(m+1). */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Compute the inverse of a modulo m, 0 <= a < m, gcd(a,m)=1.
   Returns 0 if a has no inverse modulo m (should not happen here). */
static uint64_t modinv(uint64_t a, uint64_t m)
{
    int64_t t = 0, newt = 1;
    int64_t r = m, newr = a;
    while (newr != 0) {
        uint64_t q = r / newr;
        int64_t tmp = t - (int64_t)q * newt;
        t = newt; newt = tmp;
        tmp = r - (int64_t)q * newr;
        r = newr; newr = tmp;
    }
    if (r > 1) return 0;                 /* not invertible – impossible */
    if (t < 0) t += m;
    return (uint64_t)t;
}

/* Factor n into an array of prime powers p^e and return the count.
   powers[i] contains the value p^e for the i‑th distinct prime factor. */
static size_t factor_prime_powers(uint64_t n, uint64_t powers[20])
{
    size_t cnt = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t pw = 1;
            while (tmp % p == 0) {
                tmp /= p;
                pw *= p;
            }
            powers[cnt++] = pw;
        }
    }
    if (tmp > 1) powers[cnt++] = tmp;     /* remaining prime factor */
    return cnt;
}

/* Implements the sequence. */
extern seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not defined, but safe */
    if (n == 1) return (seq_t)1;  /* a(1)=1 */

    uint64_t powers[20];
    size_t k = factor_prime_powers((uint64_t)n, powers);

    uint64_t best_m = UINT64_MAX;
    seq_t     best_gcd = 0;

    uint64_t masks = (uint64_t)1 << k;   /* up to 2^k combinations */
    for (uint64_t mask = 0; mask < masks; ++mask) {
        uint64_t m = 0;          /* current solution modulo "mod" */
        uint64_t mod = 1;        /* current modulus (product of processed primes) */
        for (size_t i = 0; i < k; ++i) {
            uint64_t pi = powers[i];
            uint64_t residue = ((mask >> i) & 1) ? (pi - 1) : 0;

            /* Solve: m' ≡ m (mod mod) and m' ≡ residue (mod pi) */
            uint64_t diff = (residue + pi - m % pi) % pi;
            uint64_t inv  = modinv(mod % pi, pi);    /* inverse of mod modulo pi */
            uint64_t t    = (__uint128_t)diff * inv % pi;

            m += (__uint128_t)mod * t;
            mod *= pi;
            m %= mod;   /* keep m within 0..mod-1 */
        }

        uint64_t m_mod = m % n;       /* m modulo n in [0,n-1] */
        uint64_t m_val = (m_mod == 0) ? n : m_mod;  /* smallest positive m */

        if (m_val < best_m) {
            best_m = m_val;
            uint64_t m_plus = m_val + 1;
            best_gcd = gcd((seq_t)n, (seq_t)m_plus);
        }
    }

    return best_gcd;
}
