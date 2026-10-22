/*
 * A324857  Numbers m > 1 such that there exists a prime divisor p of m with
 *          s_p(m) = p.
 * https://oeis.org/A324857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 2105, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute OEIS sequence A324857: numbers m > 1 such that there exists a
 * prime divisor p of m with s_p(m) = p (sum of the base‑p digits of m).
 *
 * The function executes an incremental search: candidates are examined in
 * increasing order, tested by a straightforward trial‑division factoriser,
 * and the desired terms are stored in a static table for later fast
 * retrieval.  It works for any seq_t/idx_t combination provided by the
 * build and does not require external utilities.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static storage for the generated terms. */
static seq_t *terms      = NULL;   /* array of terms */
static size_t terms_len  = 0;      /* number of terms stored */
static size_t terms_cap  = 0;      /* allocated capacity */

/* Next candidate to test after the last generated term. */
static uint64_t next_candidate = 6;   /* first possible m (>1 with >=2 primes) */

/* Ensure that the terms array has room for at least 'needed' entries. */
static void ensure_terms_capacity(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = tmp;
    terms_cap = new_cap;
}

/* Sum of the digits of x written in the given base. */
static unsigned digit_sum_base(uint64_t x, uint64_t base)
{
    unsigned sum = 0;
    while (x) {
        sum += (unsigned)(x % base);
        x /= base;
    }
    return sum;
}

/* Test whether m satisfies the definition. */
static bool matches_condition(uint64_t m)
{
    if (m < 6) return false;  /* all numbers below 6 fail the 2‑prime‑factor test */

    /* Collect distinct prime divisors of m.  We need at most a handful
       for numbers in the range of interest. */
    uint64_t pf[16];
    size_t pf_cnt = 0;

    uint64_t tmp = m;
    /* Handle factor 2 */
    if ((tmp & 1ULL) == 0) {
        pf[pf_cnt++] = 2;
        while ((tmp & 1ULL) == 0) tmp >>= 1;
    }
    /* Odd factors up to sqrt(tmp) */
    for (uint64_t d = 3; d * d <= tmp; d += 2) {
        if (tmp % d == 0) {
            pf[pf_cnt++] = d;
            while (tmp % d == 0) tmp /= d;
        }
    }
    /* Remaining prime greater than 1 */
    if (tmp > 1) {
        pf[pf_cnt++] = tmp;
    }

    /* m must have at least two distinct prime factors (not counting multiplicity). */
    if (pf_cnt < 2) return false;

    /* Check the sum‑of‑digits condition for each prime divisor. */
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = pf[i];
        if (digit_sum_base(m, p) == (unsigned)p) return true;
    }
    return false;
}

/* Return the nth term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 – return 0 for safety */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        if (matches_condition(next_candidate)) {
            ensure_terms_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return terms[n - 1];
}
