/*
 * A321910  Base-7 deletable primes (written in base 10).
 * https://oeis.org/A321910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 484, u32 n <= 808, u64 n <= 808,
 *            u128 n <= 808, big any n
 * Helpers:   decimal_to_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Deletable primes in base 7 computed by brute‑force search up to a fixed limit. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/decimal_to_base7.h"

#define LIMIT 20000000UL   /* upper bound that comfortably contains the first 10000 terms */

/* Cache for primality: 1 = prime, 0 = composite. */
static unsigned char *prime_cache = NULL;
/* Cache for deletable status: 0 = unknown, 1 = true, 2 = false. */
static unsigned char *del_cache  = NULL;
/* Precomputed powers of 7 up to the maximum needed (unused here but kept for possible extensions). */
static uint64_t pow7[20];

/* Simple sieve of Eratosthenes up to LIMIT. */
static void init_sieve(void)
{
    prime_cache = (unsigned char*)malloc((LIMIT + 1) * sizeof(unsigned char));
    del_cache   = (unsigned char*)malloc((LIMIT + 1) * sizeof(unsigned char));
    if (!prime_cache || !del_cache) {
        /* Allocation failure; abort execution as the sequence cannot be computed. */
        exit(1);
    }
    memset(prime_cache, 1, LIMIT + 1);
    prime_cache[0] = prime_cache[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i)
        if (prime_cache[i])
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                prime_cache[j] = 0;
    memset(del_cache, 0, LIMIT + 1);
    pow7[0] = 1;
    for (int i = 1; i < 20; ++i)
        pow7[i] = pow7[i - 1] * 7ULL;
}

/* Recursive test with memoization. */
static bool is_deletable(uint64_t p)
{
    if (p == 0)
        return true;                 /* empty string is allowed */
    unsigned char cached = del_cache[p];
    if (cached)
        return cached == 1;
    if (!prime_cache[p]) {
        del_cache[p] = 2;
        return false;
    }
    uint8_t digits[20];
    int len;
    decimal_to_base7(p, digits, &len);
    for (int i = 0; i < len; ++i) {
        /* Skip deletions that would produce a leading zero. */
        if (i == 0 && len > 1 && digits[1] == 0)
            continue;
        uint64_t q = 0;
        for (int j = 0; j < len; ++j)
            if (j != i)
                q = q * 7ULL + digits[j];
        if (q == 0) {                /* deletion yields empty string */
            del_cache[p] = 1;
            return true;
        }
        if (prime_cache[q] && is_deletable(q)) {
            del_cache[p] = 1;
            return true;
        }
    }
    del_cache[p] = 2;
    return false;
}

/* Sequence storage. */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Append an item to the sequence array, growing if necessary. */
static void append_seq(uint64_t val)
{
    if (seq_len >= seq_cap) {
        if (seq_cap == 0)
            seq_cap = 8U;
        else
            seq_cap <<= 1;
        seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        if (!seq) {
            /* Allocation failure; abort execution as the sequence cannot be computed. */
            exit(1);
        }
    }
    seq[seq_len++] = (seq_t)val;
}

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static uint64_t next_checked = 2U;
    static idx_t terms_found = 0;

    if (!initialized) {
        init_sieve();
        initialized = true;
    }

    while (terms_found < n) {
        /* Advance to next candidate prime. */
        while (next_checked <= LIMIT && !prime_cache[next_checked])
            ++next_checked;
        if (next_checked > LIMIT)
            break;                    /* safeguard: limit exceeded */
        if (is_deletable(next_checked)) {
            append_seq(next_checked);
            ++terms_found;
        }
        ++next_checked;
    }

    if (n == 0)
        return 0;                       /* undefined, but returning 0 for safety */

    /* If n is beyond the precomputed terms, we cannot determine the value. */
    if (n > seq_len)
        return 0;

    return seq[n - 1];
}
