/*
 * A389092  Pythagorean prime shift, variant: fully multiplicative with a(2) =
 *          3, a(A002144(n)) = A002144(1+n) and a(A002145(n)) =
 *          a(A002145(1+n)) for all n >= 1, where A002144 and A002145 give the
 *          primes of the form 4k+1 and 4k+3 respectively.
 * https://oeis.org/A389092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 728, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pythagorean prime shift (OEIS A389092)
 *
 * The sequence is fully multiplicative.
 *   a(1) = 1
 *   a(2) = 3
 *   For any prime p:
 *     if p ≡ 1 (mod 4) then a(p) = the next prime ≡ 1 (mod 4)
 *     if p ≡ 3 (mod 4) then a(p) = the next prime ≡ 3 (mod 4)
 *   For prime powers a(p^e) = a(p)^e and a(mn) = a(m)·a(n) for all m,n.
 *
 * This implementation uses trial division to factor n (n ≤ 10 000 in the
 * tests) and a simple prime search over candidates spaced by 4 to keep
 * the correct residue class.  All arithmetic is performed in the
 * generic type seq_t; no external libraries are required beyond the
 * provided primality test.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

/* power function for seq_t */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* return a(p) for prime p (p > 2) */
static inline seq_t prime_a(uint64_t p)
{
    /* p has correct residue mod 4 (1 or 3) */
    uint64_t cand = p + 4;         /* next number with same residue */
    while (!is_prime_small((seq_t)cand)) cand += 4;
    return (seq_t)cand;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* offset 1 */

    seq_t result = 1;
    uint64_t tmp = n;

    /* factor 2 */
    if ((tmp & 1) == 0) {
        idx_t exp = 0;
        while ((tmp & 1) == 0) { tmp >>= 1; ++exp; }
        seq_t a2 = 3;                   /* a(2) = 3 */
        result *= pow_seq(a2, exp);
    }

    /* odd factors */
    for (uint64_t i = 3; i * i <= tmp; i += 2) {
        if (tmp % i == 0) {
            idx_t exp = 0;
            while (tmp % i == 0) { tmp /= i; ++exp; }
            seq_t ap = prime_a(i);
            result *= pow_seq(ap, exp);
        }
    }

    /* remaining prime factor */
    if (tmp > 1) {
        seq_t ap = prime_a(tmp);
        result *= ap;
    }

    return result;
}
