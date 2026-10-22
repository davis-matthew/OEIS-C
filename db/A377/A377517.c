/*
 * A377517  The sum of the divisors of n that are terms in A276078.
 * https://oeis.org/A377517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative a(p^e) = (p^(min(π(p),e)+1)-1)/(p-1)
 * Range:     u8 n <= 16, u16 n <= 2728, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A377517 */
/* where π(p) = number of primes ≤ p. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static uint64_t *prime_list = NULL;   /* stored primes */
static size_t   prime_cap  = 0;      /* array capacity */
static size_t   prime_cnt  = 0;      /* number of primes stored */

/* ensure prime_list has room for at least |cap| primes   */
static void grow_prime_list(size_t cap)
{
    if (prime_cap >= cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    if (new_cap < cap) new_cap = cap;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* populate prime_list so that it contains all primes ≤ limit     */
static void ensure_primes_upto(uint64_t limit)
{
    if (prime_cnt && prime_list[prime_cnt - 1] >= limit) return;

    /* first prime 2 */
    if (prime_cnt == 0) {
        grow_prime_list(1);
        prime_list[prime_cnt++] = 2;
    }

    uint64_t start = prime_list[prime_cnt - 1] + 1;
    if (start <= 2) start = 3;
    if ((start & 1) == 0) ++start;        /* next odd */

    for (uint64_t x = start; x <= limit; x += 2) {
        uint64_t r = (uint64_t)floor_sqrt((seq_t)x);
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= r; ++i) {
            if (x % prime_list[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            grow_prime_list(prime_cnt + 1);
            prime_list[prime_cnt++] = x;
        }
    }
}

/* binary search: number of primes ≤ p */
static size_t pi_of_prime(uint64_t p)
{
    if (prime_cnt == 0 || prime_list[prime_cnt - 1] < p)
        ensure_primes_upto(p);
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= p) lo = mid + 1;
        else                       hi = mid;
    }
    return lo;              /* count of primes ≤ p */
}

/* fast exponentiation for seq_t */
static seq_t pow_seq_t(seq_t base, unsigned exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 1;                /* a(1) = 1 */

    /* factor n -------------------------------------------- */
    uint64_t tmp = (uint64_t)n;
    /* largest prime needed for factorization is ≤ n */
    ensure_primes_upto(tmp);
    uint64_t pf[64];
    unsigned exp[64];
    size_t pf_cnt = 0;

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            pf[pf_cnt] = p;
            exp[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (tmp > 1) {                     /* remaining prime factor */
        pf[pf_cnt] = tmp;
        exp[pf_cnt] = 1;
        ++pf_cnt;
    }
    /* multiplicative formula -------------------------------- */
    seq_t result = 1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = pf[i];
        unsigned e = exp[i];
        size_t pi = pi_of_prime(p);
        unsigned kmax = (pi < e) ? pi : e;    /* min(π(p), e) */

        seq_t num = pow_seq_t((seq_t)p, kmax + 1) - 1;   /* p^(kmax+1)-1 */
        seq_t den = (seq_t)p - 1;
        seq_t sum = num / den;                         /* exact division */

        result *= sum;
    }
    return result;
}
