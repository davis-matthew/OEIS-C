/*
 * A380943  Primes written in decimal representation by the concatenation of
 *          primes p and q such that the concatenation of q and p also forms a
 *          prime.
 * https://oeis.org/A380943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th prime p that can be written as the
 *            concatenation * of two primes a and b such that the
 *            concatenation of b and a is also prime. * The split must not
 *            introduce leading zeros in either part
 * Range:     u8 n <= 2, u16 n <= 145, u32 n <= 145, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, concat_parts, digit_count, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/concat_parts.h"
#include "../utils/digit_count.h"
#include "../utils/append_candidate.h"

/* static storage for generated terms */
static seq_t *memo_terms = NULL;        /* array of found terms */
static size_t memo_len  = 0;            /* number of stored terms     */
static size_t memo_cap  = 0;            /* capacity of memo_terms    */

/* last candidate examined, used for incremental generation */
static idx_t last_checked = 1;          /* start at 1; will be moved to 3 on first call */

/* test whether a prime p satisfies the required property */
static bool has_property(idx_t p)
{
    uint64_t d = digit_count(p);       /* number of decimal digits */
    if (d < 2) return false;           /* need at least two digits */

    uint64_t divisor = 10ULL;          /* 10^k, where k = number of digits of right part */
    for (uint64_t k = 1; k < d; ++k, divisor *= 10ULL)
    {
        idx_t low  = p % divisor;      /* right part */
        idx_t high = p / divisor;      /* left part  */

        /* low must have exactly k digits, i.e. no leading zeros */
        if (low < divisor / 10ULL)     /* divisor/10 == 10^(k-1) */
            continue;

        if (!is_prime_big(low) || !is_prime_big(high))
            continue;

        seq_t joined = concat_parts(low, high);   /* reversed concatenation b||a */
        if (is_prime_big(joined))
            return true;
    }
    return false;
}

/* generate primes and fill memo until it contains at least n terms */
static void generate_until(idx_t n)
{
    if (!memo_terms) {
        memo_terms = NULL; memo_len = 0; memo_cap = 0;
    }

    /* starting point for candidate generation */
    if (last_checked < 3)
        last_checked = 3;
    else if ((last_checked & 1) == 0)
        last_checked++;

    while (memo_len < n) {
        idx_t cand = last_checked;
        for (;;) {
            if (is_prime_big(cand) && has_property(cand)) {
                /* store the term */
                append_candidate(&memo_terms, &memo_len, &memo_cap, cand);
                last_checked = cand + 2;   /* next odd candidate */
                break;                     /* continue outer loop */
            }
            cand += 2;                     /* skip even numbers */
            if (cand == 0)                 /* overflow guard */
                break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for offset 1, but return 0 */

    generate_until(n);                  /* ensure memo has at least n terms */
    return memo_terms[n - 1];
}
