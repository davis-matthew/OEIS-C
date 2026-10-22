/*
 * A354515  Numbers k such that m - gpf(m) = k has no solution m >= 2, gpf =
 *          A006530.
 * https://oeis.org/A354515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 8119, u32 n <= 9826, u64 n <= 9826,
 *            u128 n <= 9826, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS sequence A354515.
   A term k satisfies: for every prime factor p of k, the greatest
   prime factor of k + p is not equal to p.  The function computes
   terms on demand, caching previously found values.  It uses a
   simple trial‑division factoriser based on a sieve up to 1 000 000.
   The external routine gpf (greatest prime factor) is provided by
   "../utils/gpf.h". */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gpf.h"

/* ------------ prime sieve up to 1 000 000 ------------- */
#define PRIME_LIMIT 1000000
static uint32_t primes[78498];  /* primes <= 1,000,000 */
static size_t  prime_cnt = 0;
static bool    primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    static char is_comp[PRIME_LIMIT + 1] = {0};
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt < sizeof(primes)/sizeof(primes[0]))
                primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    primes_ready = true;
}

/* ------------ cache for computed sequence terms ---------- */
static seq_t *seq_buf = NULL;   /* array of terms */
static size_t seq_len = 0;      /* number of stored terms  */
static size_t seq_cap = 0;      /* allocated capacity      */
static idx_t  last_chk = 0;     /* last candidate examined */

/* grow capacity of seq_buf */
static void grow_seq(void)
{
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ----------------- main sequence function ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    init_primes();                 /* ensure primes are ready */

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        ++last_chk;
        uint64_t k   = (uint64_t)last_chk;
        uint64_t temp = k;
        bool good = true;

        /* trial division to find all prime factors of k */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > temp) break;

            if (temp % p == 0) {
                /* p is a prime factor of k */
                if (gpf(k + (uint64_t)p) == (uint64_t)p) {
                    good = false;
                    break;
                }
                while (temp % p == 0) temp /= p;
            }
        }

        /* remaining factor > sqrt(k) */
        if (good && temp > 1) {
            uint32_t p = (uint32_t)temp;
            if (gpf(k + (uint64_t)p) == (uint64_t)p)
                good = false;
        }

        if (good) {
            if (seq_len == seq_cap) grow_seq();
            seq_buf[seq_len++] = (seq_t)k;
        }
    }

    return seq_buf[(size_t)n - 1];
}
