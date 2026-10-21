/*
 * A247122  Primes p such that the digit sum of p is an odd composite number.
 * https://oeis.org/A247122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 563, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, digit_sum, isPrimeGeneric, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A247122 – primes with odd composite digit sum
   a(n) = nth prime such that the sum of its decimal digits is odd and
   composite.  The function produces the value a(n) for any n ≥ 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/digit_sum.h"
#include "../utils/append_candidate.h"

/* static storage for already computed terms */
static seq_t *answers = NULL;      /* array of found terms */
static size_t answer_len = 0;      /* number of stored terms */
static size_t answer_cap = 0;      /* allocated capacity */

static uint64_t candidate_start = 2;   /* last tested candidate */

/* generate terms until we have at least n entries */
static void generate_terms_up_to(idx_t n)
{
    while (answer_len < n) {
        /* start searching from the next integer after last candidate */
        uint64_t p = candidate_start + 1;
        if (p <= 2) p = 2;
        if (p > 2 && (p & 1) == 0) p++;   /* make p odd if > 2 */

        /* find the next prime that satisfies the digit‑sum condition */
        for (;; p += 2) {
            /* test primality */
            if (is_prime((idx_t)p)) {
                seq_t sum = digit_sum((seq_t)p);
                if ((sum & 1) && sum > 1 && !isPrimeGeneric(sum)) {
                    /* store the prime as the next sequence element */
                    append_candidate(&answers, &answer_len, &answer_cap,
                                     (seq_t)p);
                    candidate_start = p;    /* next search starts after p */
                    break;
                }
            }
        }
    }
}

/* external function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* behaviour below offset is irrelevant */
    generate_terms_up_to(n);
    return answers[n - 1];
}
