/*
 * A270192  Numbers n for which (prime(n+1)-prime(n)) mod 3 = 2.
 * https://oeis.org/A270192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A270192:
   a(n) = the n-th prime index k such that (prime(k+1) - prime(k)) mod 3 = 2
   (so a(1) = 2 because prime(3) – prime(2) = 5 – 3 = 2 ≡ 2 (mod 3)). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*--- configuration -----------------------------------------------------*/
#define MAX_PRIMES  50000   /* enough for n up to ~10000              */
#define MAX_INDICES 20000   /* enough for n up to ~10000             */

/*--- global storage -----------------------------------------------------*/
static uint64_t primes[MAX_PRIMES];
static size_t   primes_cnt = 0;   /* number of primes stored          */
static uint64_t next_candidate = 3;   /* next odd number to test          */

static seq_t indices[MAX_INDICES];
static size_t indices_cnt = 0; /* number of qualifying indices */

/*--- utility: integer square root (floor) ------------------------------*/
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 62;  /* largest power of 4 <= x */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/*--- add the next prime to the cache ----------------------------------*/
static void add_next_prime(void)
{
    uint64_t p;

    if (primes_cnt == 0) {
        p = 2;
    } else {
        p = next_candidate;
        for (;;) {
            uint64_t lim = isqrt_u64(p);
            bool is_prime = true;
            for (size_t i = 0; i < primes_cnt && primes[i] <= lim; ++i) {
                if (p % primes[i] == 0) { is_prime = false; break; }
            }
            if (is_prime) break;
            p += 2;
        }
        next_candidate = p + 2;
    }

    if (primes_cnt < MAX_PRIMES) {
        primes[primes_cnt++] = p;
    } else {
        abort();   /* insufficient space; should not happen for n <= 10000 */
    }

    /* check gap with the previous prime, if we have at least two primes */
    if (primes_cnt >= 2) {
        uint64_t diff = primes[primes_cnt - 1] - primes[primes_cnt - 2];
        if ((diff % 3ULL) == 2ULL) {
            if (indices_cnt < MAX_INDICES) {
                /* n = index of the lower prime in the gap */
                indices[indices_cnt++] = (seq_t)(primes_cnt - 1);
            } else {
                abort(); /* insufficient space for indices */
            }
        }
    }
}

/*--- public sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for 0, return 0 */
    while (indices_cnt < (size_t)n) {
        add_next_prime();
    }
    return indices[(size_t)n - 1];
}
