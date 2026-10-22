/*
 * A347163  Primes p such that 10*p can be written as a^2+b^2 where a and b
 *          are prime.
 * https://oeis.org/A347163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1200, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A347163: primes p such that 10*p = a^2 + b^2 with a,b primes.
   The function generates terms lazily, memoizes them, and answers requests
   in any order.  The algorithm tests primes in ascending order and uses
   integer square roots to find representations. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper from the build environment */
#include "../utils/is_prime.h"

/* ------------------------------------------------------------ */
/*  Integer square root for unsigned 64‑bit values              */
/* ------------------------------------------------------------ */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 62;       /* highest power of four <= 2^64 */

    /* bring bit down to the highest power of four <= x */
    while (bit > x) bit >>= 2;

    for (; bit; bit >>= 2) {
        uint64_t next = r + bit;
        if (x >= next) {
            x -= next;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
    }
    return r;
}

/* ------------------------------------------------------------ */
/*  Dynamic storage for the sequence and for known primes       */
/* ------------------------------------------------------------ */
static seq_t *seq        = NULL;   /* found terms of the sequence       */
static size_t seq_len    = 0;      /* number of stored terms            */
static size_t seq_cap    = 0;      /* allocated capacity of seq array   */

static idx_t *primes    = NULL;   /* list of primes encountered so far */
static size_t primes_cnt = 0;      /* how many primes                  */
static size_t primes_cap = 0;      /* allocated capacity of primes array */

static idx_t next_candidate = 2;   /* next integer to test for primality */

/* Append a value to the sequence array, growing it if necessary. */
static void seq_push(seq_t v)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();
        if (seq) {
            memcpy(tmp, seq, seq_len * sizeof(seq_t));
            free(seq);
        }
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = v;
}

/* Append a prime to the primes array, growing it if necessary. */
static void primes_push(idx_t v)
{
    if (primes_cnt == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 4;
        idx_t *tmp = (idx_t *)malloc(new_cap * sizeof(idx_t));
        if (!tmp) abort();
        if (primes) {
            memcpy(tmp, primes, primes_cnt * sizeof(idx_t));
            free(primes);
        }
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = v;
}

/* Ensure that all primes up to 'limit' (inclusive) are stored. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= (primes_cnt ? primes[primes_cnt - 1] : 1))
        return;
    idx_t start = (primes_cnt ? primes[primes_cnt - 1] + 1 : 2);
    for (idx_t i = start; i <= limit; ++i)
        if (is_prime(i))
            primes_push(i);
}

/* Check whether 10*p can be written as a^2 + b^2 with a,b primes. */
static bool has_representation(idx_t p)
{
    /* Special case: 10*5 = 5^2 + 5^2 */
    if (p == 5)
        return true;

    uint64_t tenp = (uint64_t)p * 10;
    uint64_t sqrt_limit = isqrt_uint64(tenp);

    /* Ensure we have all primes up to sqrt_limit for the a loop. */
    ensure_primes_upto((idx_t)sqrt_limit);

    for (size_t i = 0; i < primes_cnt && primes[i] <= sqrt_limit; ++i) {
        uint64_t a = primes[i];
        uint64_t a2 = a * a;
        if (a2 > tenp)
            break;
        uint64_t rem = tenp - a2;
        uint64_t b = isqrt_uint64(rem);
        if (b * b == rem && is_prime(b))
            return true;
    }
    return false;
}

/* ------------------------------------------------------------ */
/*  Public sequence function                                    */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* undefined for 0, harmless for offsets starting at 1 */

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        /* Find next prime candidate. */
        while (!is_prime(next_candidate))
            ++next_candidate;

        idx_t cand = next_candidate;
        if (has_representation(cand))
            seq_push((seq_t)cand);

        ++next_candidate;   /* advance past the tested prime */
    }
    return seq[n - 1];
}
