/*
 * A346021  Primes that are the first in a run of exactly 1 emirp.
 * https://oeis.org/A346021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A346021: Primes that are the first in a run of exactly 1 emirp.
   The function computes the sequence by generating primes up to a dynamically
   increasing limit, determining emirp status, and collecting isolated emirps.
   The routine is memoised; subsequent calls reuse the pre‑computed data. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/rev.h"
#include "../utils/is_prime.h"

/* ---------- Utility: integer square root of a 64‑bit value ---------- */
static idx_t
isqrt_u64(idx_t n)           /* returns ⌊√n⌋ */
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)    low = mid + 1;
        else           high = mid - 1;
    }
    return high;               /* high < low and high*high < n < (high+1)^2 */
}

/* ---------- Sieve of Eratosthenes for all primes ≤ limit ---------- */
static void
sieve_upto(idx_t limit, uint32_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t odd_count = (limit >> 1) + 1;          /* numbers 1,3,5,…,limit */
    size_t byte_cnt = (odd_count + 7) >> 3;       /* 8 numbers per byte */

    unsigned char *bits = (unsigned char *)calloc(byte_cnt, 1);
    if (!bits) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    /* set all odd bits to 1 (candidate primes), except 1 */
    for (size_t i = 0; i < byte_cnt; ++i) bits[i] = 0xFFU;
    bits[0] &= ~1U;   /* 1 is not prime */

    idx_t limit_sqrt = isqrt_u64(limit);

    for (idx_t p = 3; p <= limit_sqrt; p += 2) {
        size_t pi = p >> 1;                 /* index of p among odds */
        if (!(bits[pi >> 3] & (1U << (pi & 7)))) continue; /* composite */

        /* cross off multiples of p, starting at p*p, step 2p to stay odd */
        for (idx_t mult = p * p; mult <= limit; mult += p << 1) {
            size_t mi = mult >> 1;
            bits[mi >> 3] &= ~(1U << (mi & 7));
        }
    }

    /* count primes and fill array */
    size_t count = 1;                        /* prime 2 */
    for (size_t i = 0; i < odd_count; ++i)
        if (bits[i >> 3] & (1U << (i & 7))) ++count;

    uint32_t *primes = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!primes) {
        free(bits);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t pos = 0;
    primes[pos++] = 2U;                       /* first prime */

    for (size_t i = 0; i < odd_count; ++i)
        if (bits[i >> 3] & (1U << (i & 7)))
            primes[pos++] = (uint32_t)((i << 1) + 1U);

    free(bits);
    *primes_out = primes;
    *cnt_out = count;
}

/* ---------- Memoisation storage ---------- */
static seq_t   *isolated_primes   = NULL;
static size_t  isolated_count    = 0;
static bool    initialized       = false;

/* ---------- Initialisation routine that grows the sieve until enough terms are found ---------- */
static void
ensure_initialized(idx_t desired_n)
{
    if (initialized) return;

    idx_t limit = 1000000;          /* start with 1 M, grow if insufficient */
    while (true) {
        uint32_t *primes_uint32 = NULL;
        size_t    prime_cnt     = 0;
        sieve_upto(limit, &primes_uint32, &prime_cnt);
        if (!primes_uint32) break;   /* allocation failure */

        /* Convert to seq_t array */
        seq_t *primes = (seq_t *)malloc(prime_cnt * sizeof(seq_t));
        for (size_t i = 0; i < prime_cnt; ++i) primes[i] = (seq_t)primes_uint32[i];

        /* Collect isolated emirps */
        size_t cap = prime_cnt / 10 + 1;
        seq_t *tmp = (seq_t *)malloc(cap * sizeof(seq_t));
        size_t tmpcnt = 0;

        bool prev_emirp = false;
        for (size_t i = 0; i < prime_cnt; ++i) {
            bool curr_emirp;
            idx_t rev_cur = rev((idx_t)primes[i]);
            if (rev_cur == (idx_t)primes[i])           /* palindrome */
                curr_emirp = false;
            else
                curr_emirp = is_prime(rev_cur);

            bool next_emirp = false;
            if (i + 1 < prime_cnt) {
                idx_t rev_next = rev((idx_t)primes[i + 1]);
                if (rev_next != (idx_t)primes[i + 1])
                    next_emirp = is_prime(rev_next);
            }

            if (curr_emirp && !prev_emirp && !next_emirp) {
                if (tmpcnt == cap) {
                    cap <<= 1U;
                    tmp = (seq_t *)realloc(tmp, cap * sizeof(seq_t));
                }
                tmp[tmpcnt++] = primes[i];
            }
            prev_emirp = curr_emirp;
        }

        free(primes_uint32);
        free(primes);

        if (tmpcnt >= (size_t)desired_n) {
            isolated_primes = tmp;
            isolated_count  = tmpcnt;
            initialized     = true;
            break;
        }

        free(tmp);               /* insufficient, try larger limit */
        limit <<= 1U;            /* double the limit for next iteration */
    }
}

/* ---------- Public sequence access ---------- */
seq_t a(idx_t n)
{
    if (n < 1U) return 0;                 /* undefined for n<1 */

    ensure_initialized(n);                /* guarantee at least n terms */

    if ((idx_t)(n - 1U) < isolated_count)
        return isolated_primes[n - 1U];
    return 0;                              /* out of computed range */
}
