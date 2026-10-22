/*
 * A381330  Numbers that are the sum of a prime and the square of a prime in
 *          more than one way.
 * https://oeis.org/A381330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A381330: Numbers that are the sum of a prime and a prime square
 * in more than one way.  The function a(n) returns the nth term, 
 * with offset 1.  The algorithm enumerates integers in increasing order
 * and counts how many representations of the form p^2 + q with p,q prime
 * exist; a term is produced when the count reaches two.
 *
 * Formula:  a(n) is the nth integer that can be written in at least two
 * distinct ways as p^2 + q, where p and q are primes.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ---------- Integer square root ---------- */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t x = (n >> 1) + 1;
    while (x > n / x) x = (x + n / x) >> 1;
    return x;
}

/* ---------- Prime generation (trial division) ---------- */
static uint64_t *primes        = NULL;
static size_t   primes_len     = 0;
static size_t   primes_cap     = 0;
static uint64_t primes_next    = 3;
static bool     primes_init    = false;

static void ensure_primes_up_to(uint64_t limit)
{
    if (!primes_init) {
        primes_cap = 8;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) return;
        primes[0] = 2;
        primes_len = 1;
        primes_next = 3;
        primes_init = true;
    }

    while (primes_next <= limit) {
        bool prime = true;
        for (size_t i = 0; i < primes_len &&
             (uint64_t)primes[i] * primes[i] <= primes_next; ++i) {
            if (primes_next % primes[i] == 0) { prime = false; break; }
        }
        if (prime) {
            if (primes_len == primes_cap) {
                primes_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                                    primes_cap * sizeof(uint64_t));
                if (!tmp) return;
                primes = tmp;
            }
            primes[primes_len++] = primes_next;
        }
        ++primes_next;
    }
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    uint64_t limit = isqrt_u64(n);
    ensure_primes_up_to(limit);
    for (size_t i = 0; i < primes_len && primes[i] <= limit; ++i) {
        if (n % primes[i] == 0) return n == primes[i];
    }
    return true;
}

/* ---------- Sequence caching ---------- */
static seq_t *seq        = NULL;
static size_t seq_len    = 0;
static size_t seq_cap    = 0;

static unsigned char *count      = NULL;
static size_t          count_cap = 0;

static uint64_t max_checked = 0;

static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) return;
    seq = tmp;
    seq_cap = new_cap;
}

static void ensure_count_capacity(size_t needed)
{
    if (needed <= count_cap) return;
    size_t new_cap = count_cap ? count_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(count, new_cap);
    if (!tmp) return;
    if (tmp != count) count = tmp;
    /* zero the new portion */
    memset(count + count_cap, 0, new_cap - count_cap);
    count_cap = new_cap;
}

static void extend_sequence_to(idx_t n)
{
    while (seq_len < (size_t)n) {
        ++max_checked;
        ensure_count_capacity(max_checked + 1);

        uint64_t limit = isqrt_u64(max_checked > 2 ?
                                   max_checked - 2 : 0);
        ensure_primes_up_to(limit);

        for (size_t i = 0; i < primes_len && primes[i] <= limit; ++i) {
            uint64_t p   = primes[i];
            uint64_t p2  = p * p;
            uint64_t diff = max_checked - p2;
            if (diff < 2) continue;
            if (is_prime_uint64(diff)) {
                unsigned char *c = &count[max_checked];
                if (*c < 2) {
                    (*c)++;
                    if (*c == 2) {
                        ensure_seq_capacity(seq_len + 1);
                        seq[seq_len++] = (seq_t)max_checked;
                    }
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    extend_sequence_to(n);
    return seq[n - 1];
}
