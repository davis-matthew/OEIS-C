/*
 * A386796  Numbers that have exactly one exponent in their prime
 *          factorization that is equal to 2.
 * https://oeis.org/A386796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A386796: numbers with exactly one exponent equal to 2 in their prime factorisation.
 * The function enumerates successive integers, testing each for the property.
 * The implementation stores previously found terms for efficient repeat lookups.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ---------- Prime sieve ----------------------------------------------- */

static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void) {
    if (primes_inited) return;
    const uint32_t LIMIT = 20000; /* sufficient for n up to about 10⁵ */
    bool *is_comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_comp) abort();

    uint32_t cap = 64;
    prime_list = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!prime_list) abort();

    for (uint32_t p = 2; p <= LIMIT; ++p) {
        if (!is_comp[p]) {
            if (prime_cnt >= cap) {
                cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = p;
            if ((uint64_t)p * p <= LIMIT) {
                for (uint32_t q = p * p; q <= LIMIT; q += p)
                    is_comp[q] = true;
            }
        }
    }
    free(is_comp);
    primes_inited = true;
}

/* ---------- Test for exactly one exponent equal to 2 ------------------- */

static bool has_one_square_exp(uint64_t x) {
    int cnt = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= x; ++i) {
        uint64_t p = prime_list[i];
        int exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }
        if (exp == 2) {
            ++cnt;
            if (cnt > 1) return false;
        }
    }
    /* remaining x > 1 has exponent 1 */
    return cnt == 1;
}

/* ---------- Sequence storage ------------------------------------------ */

#ifdef __cplusplus
static std::vector<seq_t> seq_vec;
static uint64_t last_checked = 3;
#else
static seq_t *seq_buf = NULL;
static size_t seq_cap = 0, seq_len = 0;
static uint64_t last_checked = 3;
#endif

static void ensure_seq_len(idx_t need) {
    if (need == 0) return;
    if (need <= (idx_t)
#ifdef __cplusplus
        seq_vec.size()
#else
        seq_len
#endif
        )
        return;

    init_primes();

#ifdef __cplusplus
    while (seq_vec.size() < (size_t)need) {
        ++last_checked;
        if (has_one_square_exp(last_checked))
            seq_vec.push_back((seq_t)last_checked);
    }
#else
    if (!seq_buf) {
        seq_cap = 64;
        seq_buf = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        if (!seq_buf) abort();
    }
    while (seq_len < (size_t)need) {
        ++last_checked;
        if (has_one_square_exp(last_checked)) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(seq_buf, seq_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq_buf = tmp;
            }
            seq_buf[seq_len++] = (seq_t)last_checked;
        }
    }
#endif
}

/* ---------- Public sequence function ---------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* offset starts at 1 */
    ensure_seq_len(n);
    return
#ifdef __cplusplus
        seq_vec[n - 1];
#else
        seq_buf[n - 1];
#endif
}
