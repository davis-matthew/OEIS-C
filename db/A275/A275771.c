/*
 * A275771  a(n+3) = A008578(n+1) -a(n), a(0) = a(1) = a(2) = 0.
 * https://oeis.org/A275771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+3) = A008578(n+1) – a(n) where A008578 is the non‑composite
 *            numbers 1,2,3,5,7,11,… (primes plus 1). Initial values: a(0)=0,
 *            a(1)=0, a(2)=0
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *noncomposite_arr = NULL; /* 0‑based: noncomposite[0]=1, then primes */
static size_t noncomposite_cap = 0, noncomposite_len = 0;

static uint64_t *primes_arr = NULL;    /* list of primes for primality test */
static size_t primes_cap = 0, primes_len = 0;

static uint64_t next_candidate = 2;    /* next integer to test for primality */

/* Ensure noncomposite_arr contains entries 0 … needed (inclusive) */
static void ensure_noncomposite(size_t needed)
{
    if (needed < noncomposite_len) return;

    /* Initialise noncomposite array on first use */
    if (noncomposite_len == 0) {
        noncomposite_cap = 16;
        noncomposite_arr = (seq_t*)calloc(noncomposite_cap, sizeof(seq_t));
        noncomposite_arr[0] = (seq_t)1;
        noncomposite_len   = 1;
    }

    /* Ensure primes array exists */
    if (primes_cap == 0) {
        primes_cap = 16;
        primes_arr = (uint64_t*)calloc(primes_cap, sizeof(uint64_t));
    }

    /* Generate missing primes and extend noncomposite array */
    for (size_t idx = noncomposite_len; idx <= needed; ++idx) {
        for (;;) {
            uint64_t cand = next_candidate;
            bool is_prime = true;
            for (size_t pi = 0; pi < primes_len; ++pi) {
                uint64_t p = primes_arr[pi];
                if (p > cand / p) break;   /* stop when p > sqrt(cand) */
                if (cand % p == 0) { is_prime = false; break; }
            }

            if (is_prime) {
                /* Store the prime */
                if (primes_len == primes_cap) {
                    primes_cap *= 2;
                    primes_arr = (uint64_t*)realloc(primes_arr, primes_cap * sizeof(uint64_t));
                }
                primes_arr[primes_len++] = cand;

                /* Add to noncomposite array */
                if (noncomposite_len == noncomposite_cap) {
                    noncomposite_cap *= 2;
                    noncomposite_arr = (seq_t*)realloc(noncomposite_arr,
                                                       noncomposite_cap * sizeof(seq_t));
                }
                noncomposite_arr[noncomposite_len++] = (seq_t)cand;

                ++next_candidate;
                break;   /* move to next idx */
            }
            ++next_candidate;
        }
    }
}

static seq_t *a_arr   = NULL; /* cache of a(n) values */
static size_t a_cap   = 0, a_len   = 0;   /* number of elements actually stored */

static void ensure_a(idx_t n)
{
    /* Lazy initialisation with base values a(0)=a(1)=a(2)=0 */
    if (a_arr == NULL) {
        a_cap = 3;
        a_arr = (seq_t*)calloc(a_cap, sizeof(seq_t));
        a_arr[0] = a_arr[1] = a_arr[2] = (seq_t)0;
        a_len   = 3;
    }

    if ((size_t)n < a_len) return;            /* already computed up to n */

    /* Grow a_arr if needed */
    if (a_cap <= (size_t)n + 1) {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        a_arr = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }

    /* Compute missing values using the recurrence */
    while (a_len <= (size_t)n) {
        idx_t i = a_len;                     /* index of term to compute */
        idx_t rec_index = i - 3;             /* n in the recurrence a(i)=noncomposite[rec_index]-a(rec_index) */
        ensure_noncomposite((size_t)rec_index);
        seq_t val = noncomposite_arr[rec_index] - a_arr[rec_index];
        a_arr[a_len++] = val;
    }
}

/* Public function requested in the specification */
seq_t a(idx_t n)
{
    ensure_a(n);
    return a_arr[n];
}
