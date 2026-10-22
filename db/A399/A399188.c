/*
 * A399188  Numbers x such that recursion {x -> A000005(x)+1} reaches a
 *          fixed-point of 4 rather than 3.
 * https://oeis.org/A399188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A399188: Numbers whose τ(x)+1 iteration ends at the fixed point 4 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------  dynamic arrays  -------------------- */
static seq_t *term_array = NULL;      /* stored terms  */
static size_t term_len   = 0;         /* number of terms stored */
static size_t term_cap   = 0;         /* capacity of term_array */

/* --------------------  prime sieve (on‑demand)  -------------------- */
static uint32_t *prime_list = NULL;   /* list of primes */
static size_t   prime_cnt   = 0;      /* number of primes stored */
static size_t   prime_cap   = 0;      /* capacity of prime_list */
static uint32_t prime_limit = 1;      /* greatest number for which primes are known */

/* integer square root for unsigned 64‑bit values (works safely up to 2^32‑1) */
static uint32_t isqrt_u64(uint64_t n)
{
    uint32_t r = 0;
    while ((uint64_t)(r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* Grow a dynamic array of 32‑bit integers to contain at least 'limit' elements. */
static void ensure_primes_up_to(uint32_t limit)
{
    if (limit <= prime_limit) return;

    for (uint32_t cand = prime_limit + 1; cand <= limit; ++cand) {
        if (cand < 2) continue;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt + 1 > prime_cap) {
                size_t new_cap = prime_cap ? (prime_cap << 1) : 4;
                uint32_t *newp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
                prime_list = newp;
                prime_cap  = new_cap;
            }
            prime_list[prime_cnt++] = cand;
        }
    }
    prime_limit = limit;
}

/* Compute τ(n) for a 64‑bit unsigned integer n. */
static uint32_t tau_uint64(uint64_t n)
{
    if (n <= 1) return 1;
    uint32_t result = 1;
    uint64_t m = n;
    uint32_t sqrtm = isqrt_u64(m);
    ensure_primes_up_to(sqrtm);
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= m; ++i) {
        uint32_t p = prime_list[i];
        if (m % p == 0) {
            uint32_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            result *= (e + 1);
        }
    }
    if (m > 1) result *= 2;
    return result;
}

/* Return true if n eventually reaches the fixed point 4 under x -> τ(x)+1. */
static bool reaches_fixed_point_4(uint64_t n)
{
    uint64_t cur = n;
    while (cur != 4 && cur != 3) {
        uint32_t tau_val = tau_uint64(cur);
        cur = (uint64_t)tau_val + 1ULL;
    }
    return cur == 4;
}

/* Append a term to the dynamic term array. */
static void push_term(seq_t val)
{
    if (term_len == term_cap) {
        term_cap = term_cap ? (term_cap << 1) : 8;
        term_array = (seq_t *)realloc(term_array, term_cap * sizeof(seq_t));
    }
    term_array[term_len++] = val;
}

/* --------------------  main function required by the harness  -------------------- */
static uint64_t last_tested = 0;  /* highest integer examined so far */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* a(0) undefined, return 0 */
    if ((size_t)n <= term_len)                 /* cached result */
        return term_array[(size_t)n - 1];

    while (term_len < (size_t)n) {
        ++last_tested;
        if (reaches_fixed_point_4(last_tested))
            push_term((seq_t)last_tested);
    }
    return term_array[(size_t)n - 1];
}
