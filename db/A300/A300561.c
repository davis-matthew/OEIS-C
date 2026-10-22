/*
 * A300561  Deep factorization of n, A300560, converted from binary to
 *          decimal. (Binary digits obtained by recursively replacing each
 *          factor p^e with [primepi(p) [e]], then '[' = 1, ']' = 0.)
 * https://oeis.org/A300561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The function computes OEIS A300561 – the decimal value of the
 *            binary representation obtained from the deep factorization of n.
 *            Each prime exponent p^e is replaced by the bracketed expression
 *            [primepi(p)[e]], with '[' mapped to binary 1 and ']' to binary 0
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary headers – the build supplies seq_t and idx_t. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- Prime sieve data (shared across calls) ---*/
static uint32_t *prime_list   = NULL;
static size_t   prime_cnt     = 0;
static uint32_t prime_limit   = 0;   /* largest prime pre‑computed */

/* Helper to free prime data. */
static void free_prime_data(void) {
    if (prime_list != NULL) {
#ifdef __cplusplus
        delete[] prime_list;
#else
        free(prime_list);
#endif
        prime_list = NULL;
        prime_cnt = 0;
        prime_limit = 0;
    }
}

/* Replace all primes up to 'limit' by a sieve. */
static void build_primes_upto(uint32_t limit) {
    free_prime_data();
    size_t sz = (size_t)limit + 1;
#ifdef __cplusplus
    bool *is_prime = new bool[sz];
#else
    bool *is_prime = calloc(sz, sizeof(bool));
#endif
    for (size_t i = 0; i < sz; ++i) is_prime[i] = true;
    if (sz > 0) is_prime[0] = is_prime[1] = false;

    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t k = p * p; k <= limit; k += p) {
                is_prime[k] = false;
            }
        }
    }

    uint32_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;

    if (cnt == 0) {
        free_prime_data();
#ifdef __cplusplus
        delete[] is_prime;
#else
        free(is_prime);
#endif
        return;
    }

#ifdef __cplusplus
    prime_list = new uint32_t[cnt];
#else
    prime_list = malloc(cnt * sizeof(uint32_t));
#endif
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            prime_list[idx++] = (uint32_t)i;
        }
    }
    prime_cnt   = cnt;
    prime_limit = limit;
#ifdef __cplusplus
    delete[] is_prime;
#else
    free(is_prime);
#endif
}

/* Ensure that primes up to at least 'limit' are available. */
static void ensure_primes(uint32_t limit) {
    if (limit <= prime_limit) return;
    build_primes_upto(limit);
}

/* Find the prime index (primepi) of a prime 'p'.  It is 1‑based. */
static uint32_t prime_index(uint32_t p) {
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (prime_list[i] == p) return (uint32_t)(i + 1);
    }
    return 0;  /* should never happen for valid primes */
}

/* Factorization of 'n' into primes and exponents.
   The caller provides arrays of size at least 16. */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) {
    size_t k = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= tmp; ++i) {
        uint64_t p = (uint64_t)prime_list[i];
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pr[k] = p;
            ex[k] = e;
            ++k;
        }
    }
    if (tmp > 1) {
        pr[k]   = tmp;
        ex[k]   = 1;
        ++k;
    }
    *cnt = k;
}

/* A small structure to carry a value and its binary length. */
typedef struct {
    seq_t val;
    unsigned len;
} bv_t;

/* Recursive computation of the deep factorization value for integer m. */
static bv_t deep_rec(uint64_t m) {
    if (m == 1) {
        bv_t res = { (seq_t)0, 0 };
        return res;
    }

    /* Factor m. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t   cnt;
    factor_u64(m, primes, exps, &cnt);

    /* Accumulate the total value and length. */
    seq_t         total_val = (seq_t)0;
    unsigned      total_len = 0;

    for (size_t idx = 0; idx < cnt; ++idx) {
        uint64_t p   = primes[idx];
        uint64_t e   = exps[idx];
        uint32_t pi  = prime_index((uint32_t)p);

        /* Recursively obtain the deep representations. */
        bv_t fi = deep_rec((uint64_t)pi);
        bv_t fe = deep_rec(e);

        /* Build the bit sequence for this factor:
           '['  + fi  + '[' + fe + ']' + ']'  */
        seq_t factor_val = (seq_t)0;
        unsigned factor_len = 0;

        /* '[' : binary 1 */
        factor_val = (factor_val << 1) | (seq_t)1;
        factor_len += 1;

        /* fi */
        factor_val = (factor_val << fi.len) | fi.val;
        factor_len += fi.len;

        /* '[' : binary 1 */
        factor_val = (factor_val << 1) | (seq_t)1;
        factor_len += 1;

        /* fe */
        factor_val = (factor_val << fe.len) | fe.val;
        factor_len += fe.len;

        /* closing ']' : binary 0 for exponent */
        factor_val = (factor_val << 1) | (seq_t)0;
        factor_len += 1;

        /* closing ']' : binary 0 for index */
        factor_val = (factor_val << 1) | (seq_t)0;
        factor_len += 1;

        /* Append this factor to the total. */
        total_val = (total_val << factor_len) | factor_val;
        total_len += factor_len;
    }

    bv_t res = { total_val, total_len };
    return res;
}

/* Public sequence function. */
seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;     /* a(1) = 0 by convention */
    uint32_t N = (uint32_t)n;

    /* Prepare prime data up to N (ensures primepi and factorization). */
    ensure_primes(N);

    bv_t r = deep_rec((uint64_t)N);
    return r.val;
}
