/*
 * A386167  Primes having only {2, 6, 8, 9} as digits.
 * https://oeis.org/A386167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Sequence A386167 – primes that consist only of the decimal
 *            digits 2, 6, 8, 9. The function generates all numbers made of
 *            those digits in ascending order, tests each for primality with
 *            is_prime_big, and caches the prime results. Subsequent calls
 *            reuse the cache and continue the search if necessary
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 236, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: This implementation uses only integer arithmetic
   and the helper is_prime_big(seq_t).  It buffers
   candidates and primes in dynamic arrays that are
   correctly grown with seq_realloc, making it safe for
   both native and arbitrary‑precision seq_t types. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"    /* provides bool is_prime_big(seq_t) */
#include "../utils/seq_realloc.h"    /* provides seq_t *seq_realloc(seq_t*,size_t,size_t) */

/* ----- static buffers ----------------------------------------------------- */
static seq_t *candidate_buf = NULL;   /* all allowed‑digit numbers */
static size_t cand_len = 0, cand_cap = 0;
static seq_t *prime_buf = NULL;      /* primes found so far */
static size_t prime_len = 0, prime_cap = 0;
static size_t cand_next = 0;         /* next candidate to test */

/* ----- single‑digit digits allowed ---------------------------------------- */
static const seq_t DIGITS[4] = { (seq_t)2, (seq_t)6, (seq_t)8, (seq_t)9 };

/* ----- push a new element into a seq_t buffer -------------------------------- */
static void push_buf(seq_t **buf, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 512;
        seq_t *tmp = seq_realloc(*buf, *cap, new_cap);
        if (!tmp) exit(1);
        *buf = tmp;
        *cap = new_cap;
    }
    (*buf)[(*len)++] = val;
}

/* ----- recursive generator of all length‑`len` numbers with allowed digits ------ */
static void gen_len(int pos, int len, seq_t acc)
{
    if (pos == len) {
        push_buf(&candidate_buf, &cand_len, &cand_cap, acc);
        return;
    }
    for (int i = 0; i < 4; ++i) {
        gen_len(pos + 1, len, acc * (seq_t)10 + DIGITS[i]);
    }
}

/* ----- initialise candidate buffer once ------------------------------------- */
static void init_candidates(void)
{
    if (candidate_buf) return; /* already initialised */

    /* 10 digits is enough for >10 000 primes (4^10 ≈ 1 048 576 candidates). */
    for (int len = 1; len <= 10; ++len) {
        gen_len(0, len, (seq_t)0);
    }
}

/* ----- main function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n is 1‑based */
    init_candidates();

    /* Return a cached prime if already computed. */
    if (prime_len >= (size_t)n) {
        return prime_buf[n - 1];
    }

    /* Generate further primes until we have the nth one. */
    while (prime_len < (size_t)n) {
        if (cand_next >= cand_len) {
            /* This should never happen for n ≤ 10 000 with length 10. */
            break;
        }

        seq_t cand = candidate_buf[cand_next++];
        if (is_prime_big(cand)) {
            push_buf(&prime_buf, &prime_len, &prime_cap, cand);
        }
    }

    /* After generation we must have enough primes. */
    return prime_buf[n - 1];
}
