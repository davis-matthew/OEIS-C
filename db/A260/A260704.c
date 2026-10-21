/*
 * A260704  Number of pairs of distinct divisors of A260703(n) having the
 *          property that the reversal of one is equal to the other.
 * https://oeis.org/A260704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cap_size, ensure_num_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A260704: Number of pairs of distinct divisors of A260703(n)
    having the property that the reversal of one is equal to the other.
    Formula: a(n) = number of pairs (d1,d2) with d1 | A260703(n),
                 d2 | A260703(n), d2 = rev(d1), d1 < d2. */

/*  The implementation searches integers in increasing order, finds
    those that have at least one reversal pair, counts all such pairs
    for that integer and stores the result.  Results are cached, so
    the function works for any call order.  The code is
    self‑contained, relies only on standard C headers and the
    seq_t/idx_t types supplied by the build system. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/ensure_cap_size.h"
#include "../utils/ensure_num_cap.h"

/* Prime sieve -------------------------------------------------------------*/
static uint32_t *prime_list = NULL;   /* primes up to limit */
static size_t prime_cnt   = 0;        /* number of primes stored */
static bool  primes_ready = false;

static void generate_primes(size_t limit) {
    if (primes_ready) return;
    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_comp) abort();
    prime_cnt = 0;
    uint32_t *tmp = NULL;
    size_t cap = limit / 10 + 10;
    prime_list = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!prime_list) abort();
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt == cap) {
                cap *= 2;
                uint32_t *newp = (uint32_t *)realloc(prime_list, cap * sizeof(uint32_t));
                if (!newp) abort();
                prime_list = newp;
            }
            prime_list[prime_cnt++] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    primes_ready = true;
    free(is_comp);
}

/* Reverse decimal digits ---------------------------------------------------*/
static inline uint64_t rev_uint64(uint64_t x) {
    uint64_t r = 0;
    while (x) {
        r = r * 10 + x % 10;
        x /= 10;
    }
    return r;
}

/* Factorize n into primes and exponents ----------------------------------*/
static void factor_uint64(uint64_t n, uint64_t *primes_f, uint8_t *exps_f,
                          size_t *fcnt) {
    *fcnt = 0;
    generate_primes(2000000); /* 2·10^6 > sqrt(10^12) */
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            uint8_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes_f[*fcnt] = p;
            exps_f[*fcnt]   = e;
            ++(*fcnt);
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        primes_f[*fcnt] = tmp;
        exps_f[*fcnt]   = 1;
        ++(*fcnt);
    }
}

/* Generate all divisors of n (using its factorization) --------------------*/
static uint64_t *gen_divisors(uint64_t n, size_t *divcnt) {
    uint64_t primes_f[32];
    uint8_t  exps_f[32];
    size_t fcnt;
    factor_uint64(n, primes_f, exps_f, &fcnt);

    /* compute total number of divisors */
    size_t total = 1;
    for (size_t i = 0; i < fcnt; ++i)
        total *= (size_t)(exps_f[i] + 1);

    uint64_t *divs = (uint64_t *)malloc(total * sizeof(uint64_t));
    if (!divs) abort();

    /* build divisors iteratively */
    divs[0] = 1;
    *divcnt = 1;
    for (size_t i = 0; i < fcnt; ++i) {
        size_t cur_len = *divcnt;
        uint64_t p = primes_f[i];
        uint64_t mult = 1;
        for (uint8_t e = 1; e <= exps_f[i]; ++e) {
            mult *= p;
            for (size_t j = 0; j < cur_len; ++j) {
                divs[*divcnt] = divs[j] * mult;
                ++(*divcnt);
            }
        }
    }
    return divs;
}

/* Count reverse divisor pairs for a single number ------------------------*/
static seq_t count_rev_pairs(uint64_t n) {
    size_t divcnt = 0;
    uint64_t *divs = gen_divisors(n, &divcnt);
    seq_t pairs = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divs[i];
        uint64_t rev = rev_uint64(d);
        if (rev > d && n % rev == 0) {
            ++pairs;
        }
    }
    free(divs);
    return pairs;
}

/* Cached sequence -----------------------------------------------------------*/
/* a_vals holds a(n) values                                       */
static seq_t *a_vals  = NULL;
static size_t a_len   = 0;   /* number of cached a(n) values          */
static size_t a_cap   = 0;   /* allocated capacity of a_vals          */

/* a260703_vals holds the numbers whose divisor pairs were counted   */
static uint64_t *a260703_vals = NULL;
static size_t    num_len      = 0; /* number of stored A260703 values     */
static size_t    num_cap      = 0;

/* Main public function -----------------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* not used, offset is 1           */
    /* Ensure we have computed values up to index n */
    while (a_len < (size_t)n) {
        uint64_t candidate = (num_len == 0) ? 1
                                            : (a260703_vals[num_len - 1] + 1);
        /* search for next number having at least one reverse pair   */
        while (true) {
            seq_t pairs = count_rev_pairs(candidate);
            if (pairs > 0) {
                /* store the candidate number and the pair count   */
                ensure_num_cap(&num_cap, &a260703_vals, num_len + 1);
                a260703_vals[num_len++] = candidate;
                ensure_cap_size(&a_cap, &a_vals, a_len + 1);
                a_vals[a_len++] = pairs;
                break;
            }
            ++candidate;
        }
    }
    return a_vals[(size_t)n - 1];
}
