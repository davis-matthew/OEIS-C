/*
 * A365535  Composite numbers k such that the core and the kernel of k are
 *          equal.
 * https://oeis.org/A365535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n-th composite number whose prime exponents are all odd.
   These numbers satisfy A365535: all prime power exponents are odd.  Example terms:
   6, 8, 10, 14, 15, 21, 22, 24, ... */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* static storage for the computed terms */
static seq_t *seq = NULL;          /* array of terms */
static size_t seq_len = 0;         /* number of computed terms */
static size_t seq_cap = 0;         /* allocated capacity */

/* static storage for the smallest prime factor table */
static uint64_t *spf = NULL;       /* array of smallest prime factors */
static uint64_t spf_len = 0;       /* array length (= current limit + 1) */

/* next candidate to examine */
static uint64_t next_check = 4;

/* Build the smallest prime factor table up to 'limit' (inclusive). */
static void build_spf(uint64_t limit)
{
    uint64_t len = limit + 1;
    uint64_t *new_spf = (uint64_t *)realloc(spf, len * sizeof(uint64_t));
    if (!new_spf) abort();
    spf = new_spf;
    memset(spf, 0, len * sizeof(uint64_t));
    if (len > 0) spf[0] = 0;
    if (len > 1) spf[1] = 1;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i <= limit / i) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_len = len;
}

/* Ensure the spf table covers at least 'limit' (i.e., indices 0..limit). */
static void ensure_spf(uint64_t limit)
{
    if (spf_len > limit) return;               /* already enough */
    uint64_t new_limit = spf_len;               /* current maximum index + 1 */
    if (new_limit == 0) new_limit = 1;
    /* double until the new_limit exceeds the required limit + 1 */
    while (new_limit - 1 <= limit) {
        uint64_t tmp = new_limit * 2;
        if (tmp == 0) break;                    /* overflow guard */
        new_limit = tmp;
    }
    build_spf(new_limit - 1);
}

/* Test whether a composite number has all prime exponents odd. */
static bool is_valid_composite(uint64_t n)
{
    if (n < 4) return false;                  /* exclude 1 and primes */
    /* exclude primes by checking smallest prime factor */
    if (spf[n] == n) return false;            /* prime */
    uint64_t temp = n;
    while (temp > 1) {
        uint64_t p = spf[temp];
        if (p == 0) p = temp;                 /* safety: treat as prime */
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if ((exp & 1) == 0) return false;     /* even exponent */
    }
    return true;
}

/* Store a new term */
static void add_term(uint64_t value)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 8 : seq_cap * 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq) abort();
        seq_cap = new_cap;
    }
    seq[seq_len++] = (seq_t)value;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, return 0 */

    /* initialise small spf table if first call */
    if (spf_len == 0) ensure_spf(100);

    /* generate terms up to the requested index */
    while (seq_len < n) {
        /* make sure spf covers the current candidate */
        if (next_check >= spf_len) ensure_spf(next_check);

        if (is_valid_composite(next_check)) {
            add_term(next_check);
        }
        ++next_check;
    }
    return seq[n - 1];
}
