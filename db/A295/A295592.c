/*
 * A295592  Numbers k such that Bernoulli number B_{k} has denominator 64722.
 * https://oeis.org/A295592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 49, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, isPrimeGeneric, is_target_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of A295592: numbers k such that Bernoulli number B_k
 * has denominator 64722.  Equivalently k must be a multiple of 66 and no
 * prime p other than 2, 3, 7, 23, 67 satisfies p−1 | k.  The function
 * generates such k by testing each candidate k = 66 * m (m ≥ 1) until it
 * has found n terms.  Cached results are kept in a static array for
 * subsequent calls. */

#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"        /* void factor_u64(uint64_t n,
                                           uint64_t *pr, uint64_t *ex,
                                           size_t *cnt); */
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t);      */
#include "../utils/is_target_prime.h"

static const seq_t target_primes[5] = {2, 3, 7, 23, 67};

/* Divisors of 66: 1,2,3,6,11,22,33,66 */
static const seq_t base_divs[8] = {1, 2, 3, 6, 11, 22, 33, 66};

/* Cached sequence values. */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static idx_t next_m = 1;   /* m of the next candidate k = 66*m */

/* Temporary storage for divisors of m.  For m ≤ 2¹⁶⁰ the number of divisors
 * is far less than 4096, so this  static array is safe. */
static seq_t local_divs[4096];
static size_t local_cnt = 0;

/* Recursive generation of all divisors of the integer represented by
 * primes[i]^(exp[i]), i=0..len-1.  cur is the product formed so far. */
static void gen_divs_rec(const uint64_t *primes, const uint64_t *exps,
                         int len, int idx, seq_t cur)
{
    if (idx == len) {
        local_divs[local_cnt++] = cur;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t e = exps[idx];
    seq_t val = cur;
    for (uint64_t i = 0; i <= e; ++i) {
        gen_divs_rec(primes, exps, len, idx + 1, val);
        val *= (seq_t)p;
    }
}

/* Main entry function.  Computes a(n) for n ≥ 1; a(0) is undefined
 * (returns 0 as a guard value). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety for the offset */
    /* Generate terms until we have n of them. */
    while (terms_len < (size_t)n) {
        /* 1.  Factor m = next_m. */
        uint64_t pf[64];
        uint64_t pw[64];
        size_t pf_cnt = 0;
        factor_u64((uint64_t)next_m, pf, pw, &pf_cnt);

        /* 2.  Enumerate all divisors of m. */
        local_cnt = 0;
        gen_divs_rec(pf, pw, (int)pf_cnt, 0, (seq_t)1);

        /* 3.  For each divisor d = d_m * d_66 of k = 66*m, test whether
         *     d+1 is prime and not in the target set.  If any such prime
         *     exists, the candidate is rejected. */
        bool ok = true;
        for (size_t i = 0; i < local_cnt && ok; ++i) {
            seq_t m_div = local_divs[i];
            for (int b = 0; b < 8 && ok; ++b) {
                seq_t d = base_divs[b] * m_div;   /* divisor of k */
                seq_t p = d + (seq_t)1;           /* candidate prime */
                if (isPrimeGeneric(p) && !is_target_prime(p)) {
                    ok = false;  /* forbidden prime found */
                }
            }
        }

        /* 4.  If candidate passed all tests, store it. */
        if (ok) {
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(terms,
                                               terms_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                terms = tmp;
            }
            terms[terms_len++] = (seq_t)66 * (seq_t)next_m;
        }

        ++next_m;
    }
    return terms[n - 1];
}
