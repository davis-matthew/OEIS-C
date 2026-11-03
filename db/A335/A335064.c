/*
 * A335064  Let m = d*q + r be the Euclidean division of m by d. The terms m
 *          of this sequence satisfy that q, r, d are consecutive positive
 *          integer terms in a geometric progression with a noninteger common
 *          ratio > 1.
 * https://oeis.org/A335064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A335064:   a(n) = A024619(n) * (A024619(n)+1)
 *
 *  A024619 consists of all positive integers k for which 
 *  there exist integers p, s, q' with
 *          p > s ≥ 2 ,  gcd(p,s)=1,
 *          k = p * s * q'.
 *
 *  A brute‑force test for k is implemented: 
 *  For each divisor s (2 ≤ s < √k) that divides k with s² < k,
 *    let t = k / s.  
 *    If t has any divisor p > s that is coprime with s,
 *    then k ∈ A024619.
 *
 *  The divisor test is performed by factorising t and taking the
 *  product of those prime powers that are not contained in the
 *  prime factorisation of s.  If that maximal product exceeds s,
 *  the desired divisor exists.
 *
 *  The function enumerates k in increasing order until the n‑th
 *  member of A024619 is found and returns its oblong number.
 *
 *  Complexity:   O(N √K) with very small constants; fast for
 *  n ≤ 10 000.
 */

#include <stdint.h>
#include <stdbool.h>

/* auxiliary: check whether t has a divisor > s coprime with s */
static bool divisor_greater_than_s(uint64_t t, uint64_t s,
                                   const uint64_t *s_primes, int s_cnt)
{
    uint64_t product = 1;          /* product of selected prime powers */
    uint64_t temp = t;

    /* trial division up to √t */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* skip if p divides s */
            bool skip = false;
            for (int i = 0; i < s_cnt; ++i)
                if (s_primes[i] == p) { skip = true; break; }
            if (!skip) {
                /* multiply p^exp into product, abort if already > s */
                for (int k = 0; k < exp; ++k) {
                    if (product > s / p) { product = s + 1; break; }
                    product *= p;
                    if (product > s) break;
                }
                if (product > s) return true;
            }
        }
    }

    /* remaining prime factor > √t */
    if (temp > 1) {
        uint64_t p = temp;
        bool skip = false;
        for (int i = 0; i < s_cnt; ++i)
            if (s_primes[i] == p) { skip = true; break; }

        if (!skip) {
            if (product > s / p) product = s + 1;
            else product *= p;
        }
    }

    return product > s;
}

/* auxiliary: factorise n and store unique prime factors in primes[] */
static void factor_unique_primes(uint64_t n, uint64_t primes[], int *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            primes[(*cnt)++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) primes[(*cnt)++] = n;
}

/* test whether k belongs to A024619 */
static bool in_A024619(uint64_t k)
{
    /* iterate over divisors s (2 ≤ s < √k) */
    for (uint64_t s = 2; s * s < k; ++s) {
        if (k % s) continue;

        uint64_t t = k / s;

        /* factor s once per candidate */
        uint64_t primes_s[16];
        int cnt_s;
        factor_unique_primes(s, primes_s, &cnt_s);

        if (divisor_greater_than_s(t, s, primes_s, cnt_s))
            return true;
    }
    return false;
}

/* Main routine requested by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* unused for offset 1 */
    uint64_t count = 0;
    uint64_t k = 1;

    while (true) {
        ++k;
        if (in_A024619(k)) {
            ++count;
            if (count == n) {
                /* return k*(k+1) in the required seq_t type   */
                return (seq_t)k * (seq_t)(k + 1);
            }
        }
    }
}
