/*
 * A247122  Primes p such that the digit sum of p is an odd composite number.
 * https://oeis.org/A247122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 563, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9411 (2026-11-03)
 * Helpers:   digit_sum, is_prime, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A247122 – primes with odd composite digit sum
   a(n) = nth prime such that the sum of its decimal digits is odd and
   composite.  The function produces the value a(n) for any n ≥ 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stddef.h>
#include <limits.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

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
