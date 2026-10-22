/*
 * A309374  Numbers k such that k+j is prime for every j, where 1 <= j < k and
 *          gcd(j,k) = 1.
 * https://oeis.org/A309374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the nth positive integer k such that k + j is prime for
 *            every 1 ≤ j < k with gcd(j,k) = 1. The sequence is finite: 2, 4,
 *            6, 10, 12. This routine computes the terms on first use and
 *            memoises them
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"      /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/is_prime.h" /* bool is_prime(idx_t x); */

static void init_terms(seq_t *terms, size_t *cnt)
{
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;

    *cnt = 0;
    for (idx_t k = 2; k <= 50; ++k) {
        bool ok = true;
        seq_t kseq = (seq_t)k;
        for (idx_t j = 1; j < k; ++j) {
            if (gcd((seq_t)j, kseq) != 1)
                continue;
            if (!is_prime(k + j)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            terms[*cnt] = kseq;
            ++(*cnt);
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t terms[10];
    static size_t term_count = 0;

    if (term_count == 0)
        init_terms(terms, &term_count);

    if (n == 0 || n > term_count)
        return (seq_t)0;
    return terms[n - 1];
}
