/*
 * A372587  Numbers k such that (sum of binary indices of k) + (sum of prime
 *          indices of k) is even.
 * https://oeis.org/A372587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, ensure_capacity, floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A372587: numbers k such that (sum of binary indices of k) +
   (sum of prime indices of k) is even.
   Computation is done by brute‑force search with memoization. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/floor_isqrt_u64.h"          /* floor_isqrt_u64(idx_t) */
#include "../utils/build_primes_upto.h"        /* build_primes_upto(idx_t, uint64_t**, size_t*) */
#include "../utils/ensure_capacity.h"          /* ensure_capacity(size_t, size_t*, void**, size_t) */

/*--- static state ---------------------------------------------------------*/
static seq_t *seq_buf   = NULL;      /* cached output values          */
static size_t seq_cap   = 0;          /* capacity of seq_buf           */
static size_t seq_len   = 0;          /* number of elements stored     */
static uint64_t next_candidate = 1;   /* next integer to test         */

static uint64_t *prime_buf = NULL;   /* primes up to current limit    */
static size_t   prime_cnt = 0;        /* number of primes in prime_buf */

/*--- prime helpers --------------------------------------------------------*/
static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_cnt == 0 || prime_buf[prime_cnt - 1] < limit) {
        if (prime_buf) {
            free(prime_buf);
            prime_buf = NULL;
        }
        size_t cnt;
        build_primes_upto(limit, &prime_buf, &cnt);
        prime_cnt = cnt;
    }
}

/* 1‑based index of a prime `p` (assume `p` is prime). */
static size_t prime_index(uint64_t p)
{
    ensure_primes_up_to(p);          /* guarantees p is present */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_buf[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* `lo` now points to `p` */
    return lo + 1;                   /* convert 0‑based -> 1‑based   */
}

/*--- qualification test -----------------------------------------------*/
static bool qualifies(uint64_t k)
{
    /* parity of sum of binary indices :
       add position i (1‑based) for each set bit.
       Only parity needed, so we toggle parity for odd positions. */
    uint8_t binary_parity = 0;
    uint8_t toggle = 1;              /* parity of current position (odd/even) */
    uint64_t t = k;
    while (t) {
        if (t & 1ULL)
            binary_parity ^= toggle;
        t >>= 1;
        toggle ^= 1;
    }

    /* parity of sum of prime indices (multiplicity counted) */
    uint8_t prime_parity = 0;
    t = k;
    if (t > 1) {
        uint64_t lim = floor_isqrt_u64(t);
        ensure_primes_up_to(lim);

        size_t idx = 0;
        while (idx < prime_cnt && prime_buf[idx] <= lim) {
            uint64_t p = prime_buf[idx];
            if (t % p == 0) {
                uint8_t exp = 0;
                while (t % p == 0) {
                    t /= p;
                    ++exp;
                }
                if (exp & 1) {                 /* odd exponent contributes */
                    size_t ip = idx + 1;        /* 1‑based index of prime p */
                    if (ip & 1)                 /* odd index */
                        prime_parity ^= 1;
                }
            }
            ++idx;
        }
        if (t > 1) {                    /* remaining prime factor > sqrt(original k) */
            ensure_primes_up_to(t);      /* get index for this prime */
            size_t lo = 0, hi = prime_cnt;
            while (lo < hi) {
                size_t mid = (lo + hi) >> 1;
                if (prime_buf[mid] < t)
                    lo = mid + 1;
                else
                    hi = mid;
            }
            size_t ip = lo + 1;            /* 1‑based index of the prime `t` */
            if (ip & 1)
                prime_parity ^= 1;
        }
    }

    return binary_parity == prime_parity;
}

/*--- public interface -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; undefined for n=0 */
        return (seq_t)0;

    /* Generate terms until we have at least n elements */
    while (seq_len < (size_t)n) {
        while (!qualifies(next_candidate))
            ++next_candidate;

        ensure_capacity(seq_len + 1, &seq_cap, (void **)&seq_buf,
                        sizeof(seq_t));

        seq_buf[seq_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return seq_buf[n - 1];
}
