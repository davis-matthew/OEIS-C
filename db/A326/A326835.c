/*
 * A326835  Numbers whose divisors have distinct values of the Euler totient
 *          function (A000010).
 * https://oeis.org/A326835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_t, gather_gen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A326835: numbers k whose proper divisors have distinct Euler
   totient values. This implementation incrementally tests odd integers
   for the property, memoises the found terms, and returns the n‑th
   term on demand.  It works for both C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cmp_seq_t.h"
#include "../utils/gather_gen.h"

/* Test whether a given integer k satisfies the distinct phi condition. */
static bool is_term(uint64_t k)
{
    if (k == 0) return false;
    /* factorisation of k */
    uint64_t primes[16];
    int exps[16];
    int nf = 0;
    uint64_t t = k;
    if (t % 2 == 0) {
        int cnt = 0;
        while (t % 2 == 0) { t /= 2; cnt++; }
        primes[nf] = 2; exps[nf++] = cnt;
    }
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            int cnt = 0;
            while (t % p == 0) { t /= p; cnt++; }
            primes[nf] = p; exps[nf++] = cnt;
        }
    }
    if (t > 1) {
        primes[nf] = t; exps[nf++] = 1;
    }

    seq_t phis[256];
    size_t pcnt = 0;
    gather_gen(0, (seq_t)1, primes, exps, nf, phis, &pcnt);

    qsort(phis, pcnt, sizeof(seq_t), cmp_seq_t);
    for (size_t i = 1; i < pcnt; ++i) {
        if (phis[i] == phis[i - 1]) return false;
    }
    return true;
}

/* Return the n-th term of the sequence (n ≥ 1).  The function
   memoises already found terms and builds them lazily. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, handled with a zero return */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t next_k = 1;

    while (terms_len < (size_t)n) {
        uint64_t k = next_k++;
        if (k > 1 && (k & 1ULL) == 0) continue; /* skip evens >1 */
        if (is_term(k)) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap << 1 : 16;
                seq_t *new_ptr = (seq_t *)realloc(terms,
                                                  new_cap * sizeof(seq_t));
                if (!new_ptr) exit(1);
                terms = new_ptr;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)k;
        }
    }
    return terms[n - 1];
}
