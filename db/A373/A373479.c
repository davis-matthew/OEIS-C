/*
 * A373479  Numbers k such that A001414(k) and A003415(k) are both multiples
 *          of 3, but A083345(k) is not, where A001414 is the sum of prime
 *          factors with repetition, A003415 is the arithmetic derivative, and
 *          A083345(n) = A003415(n)/gcd(n,A003415(n)).
 * https://oeis.org/A373479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 587, u32 n <= 8159, u64 n <= 8159,
 *            u128 n <= 8159, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that A001414(k) and A003415(k) are both multiples of 3,
   but A083345(k) is not.  The function returns the n‑th term (1‑based). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* dynamic array of seq_t values */
static seq_t *terms = NULL;
static size_t term_cnt = 0;   /* number of terms stored   */
static size_t term_cap = 0;   /* allocated capacity       */
static uint64_t max_checked = 0; /* largest k examined so far */

/* test whether k satisfies the conditions */
static bool qualifies(uint64_t k)
{
    uint64_t n = k;
    seq_t sumPrime = 0;
    seq_t D = 0;
    uint64_t orig = k;

    /* trial division factorisation */
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            unsigned int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            sumPrime += (seq_t)exp * (seq_t)p;              /* sum of prime factors with multiplicity */
            D += (seq_t)(orig / p) * (seq_t)exp;           /* arithmetic derivative contribution   */
        }
    }
    /* remaining prime factor (if any) */
    if (n > 1) {
        sumPrime += (seq_t)n;
        D += (seq_t)(orig / n);                            /* exponent 1 */
    }

    /* conditions: sumPrime ≡ 0 (mod 3) and D ≡ 0 (mod 3) but D/gcd(k,D) not ≡ 0 (mod 3) */
    const seq_t mod3 = (seq_t)3;
    if (sumPrime % mod3 != 0) return false;
    if (D % mod3 != 0) return false;

    seq_t g = gcd((seq_t)orig, D);
    seq_t reduced = D / g;
    if (reduced % mod3 == 0) return false;

    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n==0, safety */

    /* extend the sequence until it contains n terms */
    while (term_cnt < n) {
        ++max_checked;
        if (qualifies(max_checked)) {
            if (term_cnt >= term_cap) {
                /* grow the storage */
                term_cap = (term_cap == 0) ? 8 : term_cap * 2;
                terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
            }
            terms[term_cnt++] = (seq_t)max_checked;
        }
    }
    return terms[n - 1];
}
