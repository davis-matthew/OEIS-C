/*
 * A325355  One plus the number of steps applying A325351 (Heinz number of
 *          augmented differences of reversed prime indices) to reach a fixed
 *          point.
 * https://oeis.org/A325355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps applying A325351 to reach a fixed point,
 *            plus one. Implementation uses the definition of A325351: for n >
 *            1, let p1^e1 ... pk^ek be the prime factorisation of n. Form the
 *            multiset of prime indices where each prime index i appears ei
 *            times. Reverse this list to obtain r1 ≥ r...
 * Range:     u8 n <= 402, u16 n <= 9236, u32 n <= 9236, u64 n <= 9236,
 *            u128 n <= 9236, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* seq_t and idx_t are defined by the build environment. */
static bool primes_initialized = false;
static uint32_t *primes = NULL;
static size_t prime_count = 0;
static uint32_t *prime_to_index = NULL; /* prime_to_index[p] = index of prime p (1‑based) */

static void init_primes(void)
{
    if (primes_initialized) return;
    const int LIMIT = 2000000;
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    for (int i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    int sqrtL = (int)sqrt((double)LIMIT);
    for (int i = 2; i <= sqrtL; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
        }
    }
    /* count primes */
    size_t count = 0;
    for (int i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++count;
    primes = (uint32_t *)malloc(count * sizeof(uint32_t));
    prime_to_index = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    for (int i = 0; i <= LIMIT; ++i) prime_to_index[i] = 0;
    size_t idx = 0;
    for (int i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            primes[idx] = (uint32_t)i;
            prime_to_index[i] = (uint32_t)(idx + 1); /* 1‑based index */
            ++idx;
        }
    }
    prime_count = count;
    primes_initialized = true;
    free(is_prime);
}

/* Apply one step of A325351 to n (n is assumed to fit in uint64_t). */
static seq_t apply_mapping(seq_t n_val)
{
    if (n_val == (seq_t)0 || n_val == (seq_t)1) return (seq_t)1;
    init_primes();

    uint64_t n = (uint64_t)n_val;
    uint64_t temp = n;

    /* Collect indices of prime factors with multiplicity. */
    unsigned int idx_list[128];
    int idx_count = 0;

    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            uint32_t idx = prime_to_index[p];  /* 1‑based */
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            for (int k = 0; k < cnt && idx_count < 128; ++k) idx_list[idx_count++] = idx;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        if (temp <= 2000000u) {
            uint32_t idx = prime_to_index[(uint32_t)temp];
            if (idx_count < 128) idx_list[idx_count++] = idx;
        } else {
            /* rarely reached: compute index by linear search */
            uint32_t idx = 0;
            for (size_t i = 0; i < prime_count; ++i) {
                if (primes[i] == (uint32_t)temp) {
                    idx = (uint32_t)(i + 1);
                    break;
                }
            }
            if (idx && idx_count < 128) idx_list[idx_count++] = idx;
        }
    }

    if (idx_count == 0) return (seq_t)1;

    /* Reverse the list. */
    for (int i = 0; i < idx_count / 2; ++i) {
        unsigned int tmp = idx_list[i];
        idx_list[i] = idx_list[idx_count - 1 - i];
        idx_list[idx_count - 1 - i] = tmp;
    }

    /* Compute Heinz number of augmented differences. */
    seq_t prod = (seq_t)1;
    for (int i = 0; i < idx_count - 1; ++i) {
        int aug = (int)idx_list[i] - (int)idx_list[i + 1] + 1;
        prod *= (seq_t)primes[aug - 1];
    }
    int aug_last = (int)idx_list[idx_count - 1];
    prod *= (seq_t)primes[aug_last - 1];
    return prod;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    seq_t steps = (seq_t)0;
    seq_t cur = (seq_t)n;
    while (true) {
        seq_t next = apply_mapping(cur);
        if (next == cur) break;
        cur = next;
        ++steps;
    }
    return steps + (seq_t)1;
}
