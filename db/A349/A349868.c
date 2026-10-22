/*
 * A349868  Nobly abundant numbers (A349758) that are not abundant numbers
 *          (A005101).
 * https://oeis.org/A349868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5349, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A349868 – numbers n such that n is a nobly abundant number
   (σ(d(n)) > 2·d(n) and σ(σ(n)) > 2·σ(n)) and n is not abundant
   (σ(n) ≤ 2·n).  The function generates the sequence lazily and
   memorizes previously computed terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime sieve utilities ---------- */
static uint32_t *prime_list   = NULL;  /* array of primes, type uint32_t */
static size_t   prime_cnt     = 0;     /* number of primes stored   */
static size_t   prime_cap     = 0;     /* allocated capacity       */
static uint64_t prime_max     = 1;     /* highest number checked   */

static void grow_primes(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    prime_list = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    prime_cap  = new_cap;
}

/* Extend prime list to include all primes ≤ limit. */
static void extend_primes_up_to(uint64_t limit)
{
    if (limit <= prime_max) return;
    for (uint64_t x = prime_max + 1; x <= limit; ++x) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > x) break;
            if (x % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            grow_primes(prime_cnt + 1);
            prime_list[prime_cnt++] = (uint32_t)x;
        }
    }
    prime_max = limit;
}

/* ---------- Divisor function utilities ---------- */
/* Count of positive divisors of n (uint64_t). */
static uint64_t divisor_count_uint64(uint64_t n)
{
    uint64_t cnt = 1;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        if (tmp % prime_list[i] == 0) {
            int e = 0;
            uint32_t p = prime_list[i];
            while (tmp % p == 0) { tmp /= p; ++e; }
            cnt *= (uint64_t)(e + 1);
        }
    }
    if (tmp > 1) cnt *= 2ULL;
    return cnt;
}

/* Sum of positive divisors of n (σ(n)). */
static uint64_t divisor_sum_uint64(uint64_t n)
{
    uint64_t sum = 1;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        if (tmp % prime_list[i] == 0) {
            uint64_t part = 1;
            uint64_t pow  = 1;
            uint32_t p = prime_list[i];
            while (tmp % p == 0) {
                tmp /= p;
                pow  *= p;
                part += pow;
            }
            sum *= part;
        }
    }
    if (tmp > 1) sum *= (1ULL + tmp);
    return sum;
}

/* ---------- Sequence cache ---------- */
static seq_t *seq_terms   = NULL; /* array of computed terms */
static size_t seq_len     = 0;    /* number of terms stored   */
static size_t seq_cap     = 0;    /* allocated capacity       */
static uint64_t current_candidate = 1; /* next integer to test   */

static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;
    seq_terms = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* ---------- Main function for the sequence ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* invalid index: return 0 */

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        ++current_candidate;
        extend_primes_up_to(current_candidate);

        uint64_t d_val    = divisor_count_uint64(current_candidate);
        uint64_t sigma_val = divisor_sum_uint64(current_candidate);

        /* n itself must NOT be abundant: σ(n) ≤ 2n. */
        if (sigma_val > 2ULL * current_candidate) continue;

        /* d(n) must be abundant: σ(d(n)) > 2·d(n). */
        if (divisor_sum_uint64(d_val) <= 2ULL * d_val) continue;

        /* σ(n) must be abundant: σ(σ(n)) > 2·σ(n). */
        if (divisor_sum_uint64(sigma_val) <= 2ULL * sigma_val) continue;

        /* It qualifies – store it. */
        ensure_seq_capacity(seq_len + 1);
        seq_terms[seq_len++] = (seq_t)current_candidate;
    }

    return seq_terms[n - 1];
}
