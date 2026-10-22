/*
 * A386797  Numbers that have exactly two exponents in their prime
 *          factorization that are equal to 2.
 * https://oeis.org/A386797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers that have exactly two exponents equal to 2 in their
 *            prime factorization
 * Range:     u8 n <= 6, u16 n <= 1538, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Smallest prime factor table */
static uint64_t *spf = NULL;
static size_t spf_limit = 0;

/* Build spf up to a given limit */
static void init_spf(size_t limit) {
    if (spf) return;
    spf_limit = limit;
    spf = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    for (size_t i = 0; i <= limit; ++i) spf[i] = i;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {
            for (size_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
}

/* Count how many distinct primes have exponent exactly 2 */
static inline int count_two_exponents(uint64_t n) {
    int cnt = 0;
    while (n > 1) {
        uint64_t p = spf[n];
        int exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        if (exp == 2) ++cnt;
    }
    return cnt;
}

/* Cache of generated sequence terms */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_count = 0;

/* Grow the terms array */
static void grow_terms(size_t min_cap) {
    size_t new_cap = terms_cap ? terms_cap * 2 : 128;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    terms = tmp;
    terms_cap = new_cap;
}

/* State for incremental generation */
static bool initialized = false;
static idx_t next_candidate = 4;   /* numbers start from 4 */

/* Main function required by the build */
seq_t a(idx_t n) {
    if (!initialized) {
        init_spf(2000000);           /* 2 000 000 is safely above the values needed for n ≤ 10 000 */
        grow_terms(128);
        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (terms_count < n) {
        if (next_candidate > spf_limit) {
            /* In the unlikely event we ran out of pre‑computed spf,
               extend it by doubling the size. */
            size_t new_limit = spf_limit * 2;
            spf = (uint64_t*)realloc(spf, (new_limit + 1) * sizeof(uint64_t));
            for (size_t i = spf_limit + 1; i <= new_limit; ++i) spf[i] = i;

            size_t start = spf_limit + 1;
            for (size_t i = 2; i * i <= new_limit; ++i) {
                if (i <= spf_limit || spf[i] == i) {
                    size_t first = (i * i > start) ? i * i : ((start + i - 1) / i) * i;
                    for (size_t j = first; j <= new_limit; j += i)
                        if (spf[j] == j) spf[j] = i;
                }
            }
            spf_limit = new_limit;
        }

        if (count_two_exponents((uint64_t)next_candidate) == 2) {
            if (terms_count == terms_cap) grow_terms(terms_count + 1);
            terms[terms_count++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return terms[n - 1];
}
