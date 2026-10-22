/*
 * A362287  Hypertotient numbers: numbers k such that the set that includes k
 *          and the numbers less than k and relatively prime to k can be
 *          partitioned into two disjoint subsets of equal sum.
 * https://oeis.org/A362287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 159, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Hypertotient numbers:
   a(n) is the n-th integer k ≥ 1 such that
   k*(phi(k)+2) ≡ 0 (mod 4), with the two numbers 10 and 30 excluded.
   (Phi denotes Euler's totient function.) */

#include <stdint.h>
#include <stdlib.h>

/* Compute Euler's totient for a 64‑bit integer.  Simple trial division
   is sufficient for the values we encounter (k up to a few ten‑thousands). */
static uint64_t phi_uint64(uint64_t n)
{
    uint64_t result = n;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0) {
                temp /= p;
            }
            result -= result / p;
        }
    }
    if (temp > 1) {
        result -= result / temp;
    }
    return result;
}

/* Return the n-th hypertotient number. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe */

    /* static memoisation storage */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;     /* number of terms stored */
    static size_t terms_cap = 0;     /* allocated capacity */
    static uint64_t last_checked = 0;/* lasttested k */

    /* initialise storage on first call */
    if (!terms) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms_len = 0;
        last_checked = 0;
    }

    while (terms_len < (size_t)n) {
        ++last_checked;
        uint64_t k = last_checked;
        if (k == 10 || k == 30)            /* special exclusions */
            continue;

        uint64_t phi = phi_uint64(k);

        /* test divisibility by 4: k*(phi+2) % 4 == 0 */
        uint64_t prod_mod4 = ((k % 4) * ((phi + 2) % 4)) % 4;
        if (prod_mod4 == 0) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)k;
        }
    }

    return terms[n - 1];
}
