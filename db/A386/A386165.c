/*
 * A386165  Primes having only {2, 6, 7, 8} as digits.
 * https://oeis.org/A386165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 44, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose decimal digits belong to {2,6,7,8}.
 * This function generates candidates with those digits, tests them
 * for primality using the supplied helper is_prime_big, and memoises
 * the results.  No simple formula exists, so the implementation is
 * a straightforward search. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

#define MAX_CACHE 20000

static seq_t prime_cache[MAX_CACHE];
static size_t prime_cnt = 0;

/* generate numbers whose digits are 2,6,7,8 only */
static const uint8_t ALLOWED[4] = {2,6,7,8};
static uint8_t digs[64];
static size_t digs_len = 0;
static bool first = true;

static void inc_digits(void)
{
    size_t i = digs_len - 1;
    for (;;)
    {
        if (digs[i] < 3)
        {
            ++digs[i];
            break;
        }
        digs[i] = 0;
        if (i == 0)
        {
            memmove(digs + 1, digs, digs_len);
            digs[0] = 0;
            ++digs_len;
            break;
        }
        --i;
    }
}

static seq_t next_candidate(void)
{
    if (first)
    {
        first = false;
        digs_len = 1;
        digs[0] = 0;                /* corresponds to digit 2 */
    }

    seq_t val = 0;
    for (size_t i = 0; i < digs_len; ++i)
        val = val * 10 + (seq_t)ALLOWED[digs[i]];

    inc_digits();
    return val;
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based and n ≥ 1 */
    while (prime_cnt < (size_t)n)
    {
        seq_t cand = next_candidate();
        if (is_prime_big(cand))
        {
            if (prime_cnt < MAX_CACHE)
                prime_cache[prime_cnt++] = cand;
            /* overflow beyond MAX_CACHE is not expected for tests */
        }
    }
    return prime_cache[n - 1];
}
