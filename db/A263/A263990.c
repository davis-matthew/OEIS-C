/*
 * A263990  Nonsquare numbers k such that k and k+1 are semiprimes.
 * https://oeis.org/A263990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 2188, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: sequential sieve for semiprimes with two distinct prime factors.
   The function builds the sequence on demand, keeping the current search
   position between calls.  Complexity is essentially linear in the value
   of n, with a small prime‑factor table that grows only when required. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef seq_t seq_t;    /* provided by the build system */
typedef idx_t idx_t;    /* provided by the build system */

/* ----- Smallest‑prime‑factor table ----- */
static uint64_t *spf = NULL;   /* spf[i] = smallest prime factor of i */
static uint64_t spf_limit = 0; /* current maximum value covered in spf */

/* Grow the SPF table to at least 'limit' (inclusive). */
static void build_spf(uint64_t limit)
{
    spf = (uint64_t *)realloc(spf, (limit + 1) * sizeof(uint64_t));
    for (uint64_t i = 0; i <= limit; ++i)
        spf[i] = i;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {
            for (uint64_t j = i * i; j <= limit; j += i)
                if (spf[j] == j)
                    spf[j] = i;
        }
    }
    spf_limit = limit;
}

/* Test whether n = p*q where p and q are distinct primes. */
static inline bool semiprime_distinct(uint64_t n)
{
    if (n < 6) return false;               /* smallest distinct‐prime product is 2*3 = 6 */
    uint64_t p = spf[n];
    if (p == n) return false;              /* n is prime */
    uint64_t q = n / p;
    if (q == p) return false;              /* square of a prime */
    return spf[q] == q;                    /* q must also be prime */
}

/* ----- Sequence storage ----- */
static seq_t *seq = NULL;   /* 0‑based array of found terms */
static size_t seq_len  = 0; /* number of stored terms */
static size_t seq_cap  = 0; /* allocated capacity */

/* Current k to start searching from, remembered between calls. */
static uint64_t current_k = 2;

/* Ensure at least 'need' terms are computed.  The function expands the
   SPF table and the sequence array as necessary. */
static void ensure_sequence(idx_t need)
{
    if (seq_len >= (size_t)need) return;   /* already enough terms */

    /* Initialize the SPF table once to a safe size. */
    if (!spf_limit) {
        spf_limit = 5000000ULL;            /* 5 million covers >10 000 terms */
        build_spf(spf_limit);
    }

    /* Allocate or grow the sequence array. */
    if (!seq) {
        seq_cap = 10000;                   /* initial capacity */
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
    } else if (seq_len + 1 > seq_cap) {    /* ensure enough space for one more */
        seq_cap *= 2;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }

    /* Scan numbers starting from the last searched value. */
    uint64_t k = current_k;
    while (seq_len < (size_t)need) {
        /* Grow SPF table if k+1 lies outside current range. */
        if (k + 1 > spf_limit) {
            uint64_t new_limit = spf_limit * 2;
            build_spf(new_limit);
            continue;                     /* re‑evaluate the same k */
        }

        if (semiprime_distinct(k) && semiprime_distinct(k + 1)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)k;
        }
        ++k;
    }
    current_k = k;                          /* start next search from here */
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                  /* undefined for n<1, but required to compile */

    ensure_sequence(n);
    return seq[n - 1];
}
