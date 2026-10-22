/*
 * A344937  a(n) is the largest k such that when strings of zeros of lengths t
 *          = 1..k are inserted between every pair of adjacent digits of
 *          prime(n), the resulting numbers are all primes.
 * https://oeis.org/A344937
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum k such that inserting strings of zeros of lengths
 *            * 1..k between every pair of adjacent digits of prime(n) yields
 *            all primes. * Computation: for each n ≥ 5, find the nth prime,
 *            extract its decimal * digits, then for t = 1,2,… construct the
 *            number with t zeros between each * pair of...
 * Range:     u8 none, u16 n <= 19, u32 n <= 19, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t)            */
#include "../utils/pow10.h"          /* pow10(idx_t) -> seq_t          */

/* ---------- Prime sieve (built once) -------------------------------------- */

static const size_t PRIME_LIMIT = 350000;   /* ≥ 25 000th prime (~350 000)   */
static bool *prime_sieve = NULL;           /* sieve array, 1 = prime        */
static uint64_t *prime_list = NULL;        /* list of primes                */
static size_t prime_cnt = 0;               /* number of primes stored       */

static void init_primes(void)
{
    if (prime_list != NULL) return;      /* already initialized */

    /* 1. Allocate and fill the sieve */
    prime_sieve = (bool*)malloc(PRIME_LIMIT + 1);
    if (!prime_sieve) return;  /* out of memory – unlikely */

    for (size_t i = 0; i <= PRIME_LIMIT; ++i) prime_sieve[i] = true;
    prime_sieve[0] = prime_sieve[1] = false;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (prime_sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                prime_sieve[j] = false;
        }
    }

    /* 2. Count primes and allocate the list */
    prime_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (prime_sieve[i]) ++prime_cnt;

    prime_list = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_list) return;  /* out of memory – unlikely */

    /* 3. Store primes in order */
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (prime_sieve[i]) prime_list[idx++] = (uint64_t)i;
}

/* ---------- Helper: get the nth prime (1‑based) -------------------------- */

static uint64_t get_prime(idx_t n)
{
    init_primes();
    if (n < 1 || (size_t)n > prime_cnt) return 0;
    return prime_list[n - 1];
}

/* ---------- Main sequence function -------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 5) return 0;          /* sequence defined from n = 5 */

    /* 1. Get the nth prime (64‑bit). */
    uint64_t p = get_prime(n);
    if (p == 0) return 0;         /* safety for out‑of‑range n */

    /* 2. Extract decimal digits of p. */
    unsigned char digits[64];
    size_t len = 0;
    uint64_t tmp = p;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        unsigned char rev[64];
        size_t rlen = 0;
        while (tmp > 0) {
            rev[rlen++] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }
        for (size_t i = 0; i < rlen; ++i)
            digits[i] = rev[rlen - 1 - i];
        len = rlen;
    }

    /* 3. Test successively larger t until composite occurs. */
    seq_t maxk = 0;
    for (size_t t = 1; ; ++t) {
        seq_t val = (seq_t)digits[0];
        seq_t pow10t1 = pow10(t + 1);   /* 10^(t+1) as seq_t */

        for (size_t j = 1; j < len; ++j) {
            val = val * pow10t1 + (seq_t)digits[j];
        }

        if (!is_prime_big(val)) break;  /* composite found */
        maxk = (seq_t)t;                /* still valid */
    }

    return maxk;
}
