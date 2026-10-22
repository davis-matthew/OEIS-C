/*
 * A343306  Numbers k such that there is only 1 abundant number (A005101)
 *          among 6*k+1 through 6*k+5.
 * https://oeis.org/A343306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 2007, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A343306: the k such that among 6*k+1 … 6*k+5 exactly one is
   an abundant number.  The implementation searches k sequentially,
   checks each of the five numbers for abundance, and memoises the
   resulting sequence. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Prime list ------------------------------------------------------------ */
static uint32_t *primes      = NULL;
static size_t   primes_cnt   = 0;

/* Build a list of primes up to LIMIT using the sieve of Eratosthenes. */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    const size_t LIMIT = 1000000;          /* sufficient for the search range */
    bool *composite = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!composite) abort();

    /* Rough estimate of number of primes below LIMIT */
    primes = (uint32_t *)malloc(80000 * sizeof(uint32_t));
    if (!primes) abort();

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!composite[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    composite[j] = true;
            }
        }
    }
    free(composite);
}

/* Check whether a 64‑bit integer is abundant.  Uses the prime list. */
static bool is_abundant_u64(uint64_t x)
{
    init_primes();

    /* 2 * x as seq_t */
    seq_t twice_x = (seq_t)2 * (seq_t)x;

    seq_t sigma = (seq_t)1;          /* product of (p^e+1+…+1) terms */
    uint64_t temp = x;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > temp) break;
        if (temp % (uint64_t)p == 0) {
            seq_t term  = (seq_t)1;
            seq_t pow   = (seq_t)1;
            while (temp % (uint64_t)p == 0) {
                temp /= (uint64_t)p;
                pow *= (seq_t)p;
                term += pow;          /* term = 1 + p + p^2 + … + p^e */
            }
            sigma *= term;
            if (sigma > twice_x) return true;
        }
    }

    if (temp > 1) {                  /* remaining prime factor (exponent 1) */
        seq_t term = (seq_t)1 + (seq_t)temp;   /* 1 + p */
        sigma *= term;
    }

    return sigma > twice_x;
}

/* Sequence memoisation ----------------------------------------------- */
static seq_t *seq_vals = NULL;      /* array of found terms   */
static size_t seq_len = 0;          /* number of terms stored */
static size_t seq_cap = 0;          /* allocated size of array*/
static uint64_t next_k   = 1;       /* next k to be testable   */

static void push_term(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_vals = tmp;
        seq_cap  = new_cap;
    }
    seq_vals[seq_len++] = val;
}

/* Public entry point --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    while (seq_len < n) {
        uint64_t k = next_k++;

        int abundant_cnt = 0;
        for (int offset = 0; offset < 5; ++offset) {
            uint64_t num = 6 * k + 1 + offset;
            if (is_abundant_u64(num)) {
                ++abundant_cnt;
                if (abundant_cnt > 1) break;
            }
        }
        if (abundant_cnt == 1)
            push_term((seq_t)k);
    }

    return seq_vals[n - 1];
}
