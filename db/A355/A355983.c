/*
 * A355983  Primes whose reversal is a multiple of 4.
 * https://oeis.org/A355983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: primes whose reversal is a multiple of 4.
 * The function a(n) returns the n‑th term of that sequence,
 * starting with a(1)=23.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/rev.h"

static seq_t *terms = NULL;      /* cached terms */
static size_t terms_len = 0;     /* number of cached terms */
static size_t terms_cap = 0;     /* capacity of the cache */
static uint64_t last_checked = 2;/* last candidate examined */

/* Ensure the terms cache can hold at least 'need' elements */
static void ensure_terms_cap(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t newcap = terms_cap ? terms_cap * 2 : 16;
    while (newcap < need)
        newcap <<= 1;
    terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    terms_cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset < 1 */

    if (!terms) {          /* first call: initialise state */
        terms_cap = 0;
        terms_len  = 0;
        terms      = NULL;
        last_checked = 2;
    }

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        uint64_t candidate = last_checked + 1;        /* next number to test */

        /* Start from the next odd number ≥ 3 */
        if (candidate <= 2)
            candidate = 3;
        if ((candidate & 1) == 0)
            candidate++;                              /* make odd */

        /* Scan odd numbers, testing primality and the reversal property */
        for (;;) {
            if (is_prime((idx_t)candidate)) {
                if (((rev((idx_t)candidate) & 3ULL) == 0)) {
                    /* Prime whose reverse is divisible by 4: store it */
                    ensure_terms_cap(terms_len + 1);
                    terms[terms_len++] = (seq_t)candidate;
                    last_checked = candidate;          /* next search starts after this */
                    break;                              /* exit inner loop */
                }
            }
            candidate += 2;                            /* next odd */
        }
    }

    return terms[n - 1];
}
