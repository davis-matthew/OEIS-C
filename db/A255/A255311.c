/*
 * A255311  Decimal expansion of the prime gap constant (concatenate the sizes
 *          of prime gaps, A001223).
 * https://oeis.org/A255311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Concatenated digits of the prime gaps between consecutive primes.
    a(n) returns the n‑th decimal digit (1‑based) in the infinite string
    created by writing the gaps 1,2,4,2,4,2,4,6,… sequentially. */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdio.h>    /* snprintf */
#include <stdbool.h>  /* bool */
#include <stdint.h>   /* uint64_t */

/* Helper: test whether the sequence number is prime.  The function is
   supplied by the harness. */
#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t n); */

static char *gap_digits = NULL;   /* concatenation of all gap digits seen so far */
static size_t digits_len  = 0;    /* total number of digits in gap_digits     */
static size_t digits_cap  = 0;    /* current capacity of the buffer           */
static uint64_t prev_prime = 2;   /* last prime processed (starts at 2)        */

/* Ensure the buffer can hold at least 'needed' bytes. */
static void ensure_gap_buffer(size_t needed)
{
    if (needed <= digits_cap) return;
    size_t new_cap = digits_cap ? digits_cap << 1 : 128;
    if (new_cap < needed) new_cap = needed;
    char *new_buf = (char *)realloc(gap_digits, new_cap);
    if (!new_buf) abort();
    gap_digits = new_buf;
    digits_cap = new_cap;
}

/* Append the decimal representation of 'gap' to the buffer. */
static void append_gap_digits(uint64_t gap)
{
    /* gap is at most a handful of digits ( < 10 ) for n <= 10000,
       so a 32‑byte temporary buffer is plenty. */
    char buf[32];
    int len = snprintf(buf, sizeof buf, "%llu", (unsigned long long)gap);
    if (len <= 0) return;                 /* should never happen */
    ensure_gap_buffer(digits_len + (size_t)len);
    for (int i = 0; i < len; ++i) {
        gap_digits[digits_len++] = buf[i];
    }
}

/* Find the next prime ≥ start.  For the small ranges needed here a
   simple trial test using is_prime_small is adequate. */
static uint64_t next_prime(uint64_t start)
{
    uint64_t cand = start;
    while (!is_prime_small((seq_t)cand)) ++cand;
    return cand;
}

/* Return the n‑th digit (1‑based) of the concatenated gaps.  The
   function lazily generates primes and gaps until it has produced at
   least n digits.  Subsequent calls are efficient because the
   accumulated digits are reused. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; not used normally */

    /* Generate gaps until we have at least n digits */
    while (digits_len < (size_t)n) {
        uint64_t next = next_prime(prev_prime + 1);
        uint64_t gap  = next - prev_prime;
        prev_prime = next;
        append_gap_digits(gap);
    }

    /* Return the requested digit as a seq_t value (0‑9). */
    return (seq_t)(gap_digits[(size_t)n - 1] - '0');
}
