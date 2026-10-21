/*
 * A270311  Indices of primes ending with the same decimal digit as the
 *          previous or next prime.
 * https://oeis.org/A270311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: indices of primes ending with the same decimal digit as the
   previous or next prime.  The function generates primes in order and
   records the 1‑based positions of any prime whose last decimal digit
   matches that of an adjacent prime. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_prime.h"

#if defined(__cplusplus)
#  define REALLOC_T(ptr, newsize, type) static_cast<type*>(realloc((ptr), (newsize)))
#else
#  define REALLOC_T(ptr, newsize, type) ((type*)realloc((ptr), (newsize)))
#endif

/* Global caches */
static seq_t *matched_indices = NULL;
static size_t matched_len = 0;
static size_t matched_cap = 0;

static uint64_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

static uint8_t *matched_flags = NULL; /* 1 if prime at that position is already in matched_indices */
static size_t flags_len = 0;

/* Last prime seen; starts below the first prime so that next_prime returns 2 */
static uint64_t last_prime = 1;

/* Ensure capacity for matched_indices */
static void ensure_matched_cap(size_t min)
{
    if (matched_cap < min) {
        size_t newcap = matched_cap ? matched_cap * 2 : 128;
        if (newcap < min) newcap = min;
        seq_t *tmp = REALLOC_T(matched_indices, newcap * sizeof(seq_t), seq_t);
        if (!tmp) exit(1);
        matched_indices = tmp;
        matched_cap = newcap;
    }
}

/* Ensure capacity for primes array */
static void ensure_primes_cap(size_t min)
{
    if (primes_cap < min) {
        size_t newcap = primes_cap ? primes_cap * 2 : 256;
        if (newcap < min) newcap = min;
        uint64_t *tmp = REALLOC_T(primes, newcap * sizeof(uint64_t), uint64_t);
        if (!tmp) exit(1);
        primes = tmp;
        primes_cap = newcap;
    }
}

/* Ensure capacity for matched_flags array */
static void ensure_flags_cap(size_t min)
{
    if (flags_len < min) {
        size_t newlen = flags_len ? flags_len * 2 : 256;
        if (newlen < min) newlen = min;
        uint8_t *tmp = REALLOC_T(matched_flags, newlen * sizeof(uint8_t), uint8_t);
        if (!tmp) exit(1);
        matched_flags = tmp;
        flags_len = newlen;
    }
}

/* Append a newly found prime and check it against the previous prime. */
static void append_prime(uint64_t p)
{
    size_t old_len = primes_len;          /* number of primes before adding p */

    ensure_primes_cap(old_len + 1);
    ensure_flags_cap(old_len + 1);

    primes[old_len] = p;
    matched_flags[old_len] = 0;           /* not yet marked as matched */

    /* Current prime's 1‑based index */
    size_t curr_index = old_len + 1;

    if (old_len > 0) {                    /* a previous prime exists */
        uint8_t prev_last = (uint8_t)(primes[old_len - 1] % 10);
        uint8_t curr_last = (uint8_t)(p % 10);
        if (prev_last == curr_last) {     /* both primes match with each other */
            /* previous prime */
            if (!matched_flags[old_len - 1]) {
                matched_flags[old_len - 1] = 1;
                ensure_matched_cap(matched_len + 1);
                matched_indices[matched_len++] = (seq_t)(old_len); /* 1‑based */
            }
            /* current prime */
            if (!matched_flags[old_len]) {
                matched_flags[old_len] = 1;
                ensure_matched_cap(matched_len + 1);
                matched_indices[matched_len++] = (seq_t)curr_index;
            }
        }
    }

    primes_len = old_len + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 */

    /* Generate primes until we have at least n matched indices. */
    while (matched_len < (size_t)n) {
        uint64_t next = next_prime(last_prime);
        last_prime = next;
        append_prime(next);
    }
    return matched_indices[(size_t)n - 1];
}
