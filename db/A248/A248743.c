/*
 * A248743  Smallest prime Q such that there is a prime R such that Q*(R+1)+1
 *          or Q*(R+1)-1 or Q*(R-1)+1 or Q*(R-1)-1 is prime(n), or 0 if no
 *          such prime Q exists.
 * https://oeis.org/A248743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 6713, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A248743:  a(n) = smallest prime Q for which a prime R satisfies
 *   Q*(R+1)+1 = prime(n)
 *   Q*(R+1)-1 = prime(n)
 *   Q*(R-1)+1 = prime(n)
 *   Q*(R-1)-1 = prime(n)
 * returns 0 if no such Q exists.  The definition uses only integer
 * arithmetic and prime testing.  The implementation collects all
 * distinct prime divisors of prime(n)-1 and prime(n)+1, tests each as
 * a candidate Q, and keeps the smallest that yields a valid R.  It
 * runs in O(π(limit)) time for each call and handles up to the
 * 10 000‑th prime comfortably. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Prime utilities used by the judge */
#include "../utils/get_prime.h"          /* seq_t get_prime(idx_t k); */
#include "../utils/build_primes_upto.h"  /* void build_primes_upto(idx_t limit,
                                         uint64_t **out, size_t *cnt); */
#include "../utils/is_prime_small.h"     /* bool is_prime_small(uint64_t n); */

/* Lazy initialization of a prime list sufficient for the 10 000‑th
 * prime (≈1e5).  The list is built once and reused. */
static uint64_t *prime_list = NULL;
static size_t    prime_count = 0;
static bool      primes_ready = false;
static const idx_t PRIME_LIMIT = 200000;  /* > 110000 (10 300‑th prime) */

static void init_primes(void)
{
    if (!primes_ready) {
        build_primes_upto(PRIME_LIMIT, &prime_list, &prime_count);
        primes_ready = true;
    }
}

/* Add a prime to an array if it is not already present. */
static void add_prime(uint64_t p, uint64_t *arr, size_t *cnt)
{
    for (size_t i = 0; i < *cnt; ++i)
        if (arr[i] == p) return;
    arr[(*cnt)++] = p;
}

/* Collect all distinct prime divisors of n into arr (deduplicated
 * against current contents).  The array must have enough space for all
 * divisors (here 64 is far more than enough). */
static void collect_prime_divisors(uint64_t n, uint64_t *arr, size_t *cnt)
{
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= n; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0) {
            add_prime(p, arr, cnt);
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) add_prime(n, arr, cnt);  /* remaining prime factor */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1 */

    init_primes();

    /* Retrieve the n‑th prime; guaranteed to fit in uint64_t. */
    const seq_t p_seq = get_prime(n);
    const uint64_t p = (uint64_t)p_seq;

    /* Gather distinct prime divisors of p-1 and p+1. */
    uint64_t divs[64];
    size_t div_cnt = 0;
    collect_prime_divisors(p - 1, divs, &div_cnt);
    collect_prime_divisors(p + 1, divs, &div_cnt);

    uint64_t bestQ = 0;  /* 0 = not found yet */

    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t q = divs[i];
        uint64_t r;

        /* Case A: Q*(R+1)+1 = p  →  (p-1)/Q = R+1  */
        if ((p - 1) % q == 0) {
            r = (p - 1) / q - 1;
            if (r >= 2 && is_prime_small(r)) {
                if (bestQ == 0 || q < bestQ) bestQ = q;
                continue;
            }
        }

        /* Case C: Q*(R-1)+1 = p  →  (p-1)/Q = R-1 */
        if ((p - 1) % q == 0) {
            r = (p - 1) / q + 1;
            if (r >= 2 && is_prime_small(r)) {
                if (bestQ == 0 || q < bestQ) bestQ = q;
                continue;
            }
        }

        /* Case B: Q*(R+1)-1 = p  →  (p+1)/Q = R+1 */
        if ((p + 1) % q == 0) {
            r = (p + 1) / q - 1;
            if (r >= 2 && is_prime_small(r)) {
                if (bestQ == 0 || q < bestQ) bestQ = q;
                continue;
            }
        }

        /* Case D: Q*(R-1)-1 = p  →  (p+1)/Q = R-1 */
        if ((p + 1) % q == 0) {
            r = (p + 1) / q + 1;
            if (r >= 2 && is_prime_small(r)) {
                if (bestQ == 0 || q < bestQ) bestQ = q;
            }
        }
    }

    return bestQ ? (seq_t)bestQ : (seq_t)0;
}
