/*
 * A373443  Positive integers that cannot be written as a sum of a practical
 *          number and a hexagonal number.
 * https://oeis.org/A373443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 90, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373443: integers that cannot be written as a sum
   of a practical number and a hexagonal number.
   The function precomputes the set of such numbers up to a
   fixed limit (5 000 000), then returns the requested term. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* test if n (<= LIMIT) is practical using a smallest‑prime‑factor array */
static bool is_practical(uint64_t n, uint32_t *spf)
{
    if (n == 1) return true;              /* 1 is practical */
    uint64_t temp = n;
    bool first = true;
    uint64_t sigma = 1;                    /* product of σ(p^e) for processed primes */

    while (temp > 1) {
        uint32_t p = spf[(uint32_t)temp];  /* assume temp <= LIMIT */
        uint64_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        /* σ(p^e) = (p^(e+1)-1)/(p-1) */
        uint64_t pow_pe = 1;
        for (uint64_t i = 0; i <= e; ++i) pow_pe *= p;
        uint64_t sigma_pe = (pow_pe - 1) / (p - 1);

        if (first) {
            if (p != 2) return false;     /* first prime must be 2 */
            first = false;
        } else {
            if (p > sigma + 1) return false;
        }
        sigma *= sigma_pe;
    }
    return true;
}

seq_t a(idx_t n)
{
    static bool inited = false;
    static seq_t *terms = NULL;   /* array of the sequence values */
    static size_t terms_len = 0;  /* number of values stored */

    if (!inited) {
        /* upper bound to generate the sequence */
        const uint32_t LIMIT = 5000000U;

        /* -------- smallest prime factor sieve -------- */
        uint32_t *spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
        if (!spf) exit(EXIT_FAILURE);
        for (uint32_t i = 0; i <= LIMIT; ++i) spf[i] = 0;
        for (uint32_t i = 2; i <= LIMIT; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                if (i <= LIMIT / i) {
                    for (uint32_t j = i * i; j <= LIMIT; j += i)
                        if (spf[j] == 0) spf[j] = i;
                }
            }
        }

        /* -------- practical numbers up to LIMIT -------- */
        uint64_t *practicals = (uint64_t *)malloc((LIMIT / 2) * sizeof(uint64_t));
        if (!practicals) exit(EXIT_FAILURE);
        size_t pract_len = 0, pract_cap = LIMIT / 2;
        for (uint32_t x = 1; x <= LIMIT; ++x) {
            if (is_practical(x, spf)) {
                if (pract_len == pract_cap) {
                    pract_cap *= 2;
                    practicals = (uint64_t *)realloc(practicals, pract_cap * sizeof(uint64_t));
                    if (!practicals) exit(EXIT_FAILURE);
                }
                practicals[pract_len++] = x;
            }
        }
        free(spf);

        /* -------- hexagonal numbers (including 0) -------- */
        uint32_t *hex = (uint32_t *)malloc((LIMIT / 2) * sizeof(uint32_t));
        if (!hex) exit(EXIT_FAILURE);
        size_t hex_len = 0, hex_cap = LIMIT / 2;
        hex[hex_len++] = 0;                     /* allow zero hexagonal part */
        for (uint64_t m = 1;; ++m) {
            uint64_t h = m * (2 * m - 1);
            if (h > LIMIT) break;
            if (hex_len == hex_cap) {
                hex_cap *= 2;
                hex = (uint32_t *)realloc(hex, hex_cap * sizeof(uint32_t));
                if (!hex) exit(EXIT_FAILURE);
            }
            hex[hex_len++] = (uint32_t)h;
        }

        /* -------- mark all representable numbers -------- */
        unsigned char *sum_ok = (unsigned char *)calloc(LIMIT + 1, sizeof(unsigned char));
        if (!sum_ok) exit(EXIT_FAILURE);
        for (size_t i = 0; i < pract_len; ++i) {
            uint32_t p = (uint32_t)practicals[i];
            for (size_t j = 0; j < hex_len; ++j) {
                uint32_t h = hex[j];
                uint32_t s = p + h;
                if (s > LIMIT) break;
                sum_ok[s] = 1;
            }
        }

        /* -------- build the sequence (numbers not representable) -------- */
        terms = (seq_t *)malloc((LIMIT / 4) * sizeof(seq_t));
        if (!terms) exit(EXIT_FAILURE);
        size_t cap = LIMIT / 4;
        for (uint32_t x = 1; x <= LIMIT; ++x) {
            if (!sum_ok[x]) {
                if (terms_len == cap) {
                    cap *= 2;
                    terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                    if (!terms) exit(EXIT_FAILURE);
                }
                terms[terms_len++] = (seq_t)x;
            }
        }

        free(practicals);
        free(hex);
        free(sum_ok);

        inited = true;
    }

    if (n > 0 && (idx_t)n <= terms_len)
        return terms[(size_t)n - 1];
    return (seq_t)0;      /* undefined for out of bounds */
}
