/*
 * A374416  a(n) is the number of ways n can be written as a sum of a
 *          practical number and two nonagonal numbers.
 * https://oeis.org/A374416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function returns a(n), the number of representations of n as a sum
 * of a practical number and two nonagonal numbers (including 0 as a
 * nonagonal number).  The OEIS sequence is A374416.
 *
 * The algorithm builds tables of prime factors, practicality flags, and
 * nonagonal numbers up to the requested n, and then counts all unordered
 * pairs of nonagonal numbers (i ≤ j) whose sum plus a practical number
 * equals n.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* maximum n that has been pre‑computed so far */
static idx_t max_n = 0;

/* smallest prime factor table up to max_n */
static uint64_t *spf = NULL;

/* practical[n] = 1 if n is practical, 0 otherwise (index 0 unused) */
static uint8_t *practical = NULL;

/* nonagonal numbers up to max_n (includes 0) */
static uint64_t *nonagonal = NULL;
static size_t nonagonal_len = 0;

/* ------------------------------------------------------------------ */
/* Utility: enlarge a buffer to hold at least new_cap elements. */
static void ensure_capacity(size_t new_cap, size_t *curr_cap, void **buf,
                           size_t elem_size)
{
    if (*curr_cap >= new_cap) return;
    size_t cap = (*curr_cap == 0) ? 8 : *curr_cap;
    while (cap < new_cap) cap <<= 1;
    *buf = realloc(*buf, cap * elem_size);
    *curr_cap = cap;
}

/* ------------------------------------------------------------------ */
/* Compute smallest prime factor table up to limit. */
static void compute_spf(idx_t limit)
{
    free(spf);
    spf = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (spf[i] == (uint64_t)i) {
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == (uint64_t)j) spf[j] = i;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Helper: 128‑bit power, used only inside practicality test. */
static inline __uint128_t pow_uint128(uint64_t base, int exp)
{
    __uint128_t res = 1;
    while (exp--) res *= base;
    return res;
}

/* ------------------------------------------------------------------ */
/* Build practicality flags up to limit.  Uses the known criterion:
 * let p1 < p2 < … be the distinct prime factors of n with exponents e1…;
 * n is practical iff p1 = 2 (or n = 1) and for each i > 1,
 *      pi ≤ 1 + σ(prod_{j<i} pj^{ej}),
 * where σ(x) = product over j (< i) of (pj^{ej+1}-1)/(pj-1). */
static void compute_practical(idx_t limit)
{
    free(practical);
    practical = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    for (idx_t i = 0; i <= limit; ++i) practical[i] = 0;
    practical[1] = 1;                 /* 1 is practical by definition */

    for (idx_t n = 2; n <= limit; ++n) {
        uint64_t m = n;
        uint64_t prev_p = 0;
        int exp = 0;
        __uint128_t sigma = 1;           /* σ of processed primes */
        bool ok = true;

        while (m > 1) {
            uint64_t p = spf[m];
            if (p == prev_p) {
                ++exp;
            } else {
                if (prev_p != 0) {
                    if (prev_p > (uint64_t)(sigma + 1)) {
                        ok = false;
                        break;
                    }
                    __uint128_t term = pow_uint128(prev_p, exp + 1);
                    term = (term - 1) / (prev_p - 1);  /* (p^{e+1}-1)/(p-1) */
                    __uint128_t new_sigma = sigma * term;
                    sigma = (new_sigma > (__uint128_t)(limit + 1))
                            ? (__uint128_t)(limit + 1)
                            : new_sigma;
                }
                prev_p = p;
                exp = 1;
            }
            m /= p;
        }

        if (ok) {
            if (prev_p > (uint64_t)(sigma + 1)) {
                ok = false;
            } else {
                __uint128_t term = pow_uint128(prev_p, exp + 1);
                term = (term - 1) / (prev_p - 1);
                __uint128_t new_sigma = sigma * term;
                sigma = (new_sigma > (__uint128_t)(limit + 1))
                        ? (__uint128_t)(limit + 1)
                        : new_sigma;
            }
        }

        practical[n] = ok ? 1 : 0;
    }
}

/* ------------------------------------------------------------------ */
/* Generate nonagonal numbers up to limit.  0 is included as k = 0. */
static void compute_nonagonal(idx_t limit)
{
    size_t cap = 0;
    free(nonagonal);
    nonagonal = NULL;
    nonagonal_len = 0;
    ensure_capacity(8, &cap, (void **)&nonagonal, sizeof(uint64_t));
    nonagonal[nonagonal_len++] = 0;               /* k = 0 gives 0 */

    for (uint64_t k = 1; ; ++k) {
        uint64_t val = (7 * k * k - 5 * k) / 2;
        if (val > (uint64_t)limit) break;
        ensure_capacity(nonagonal_len + 1, &cap, (void **)&nonagonal,
                        sizeof(uint64_t));
        nonagonal[nonagonal_len++] = val;
    }
}

/* ------------------------------------------------------------------ */
/* Ensure that all tables are ready up to at least n. */
static void ensure_up_to(idx_t n)
{
    if (n <= max_n) return;
    compute_spf(n);
    compute_practical(n);
    compute_nonagonal(n);
    max_n = n;
}

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined but harmless */
    ensure_up_to(n);
    seq_t count = 0;

    for (size_t i = 0; i < nonagonal_len; ++i) {
        uint64_t ai = nonagonal[i];
        if (ai > (uint64_t)n) break;
        for (size_t j = i; j < nonagonal_len; ++j) {
            uint64_t aj = nonagonal[j];
            uint64_t sum = ai + aj;
            if (sum > (uint64_t)n) break;
            uint64_t rem = (uint64_t)n - sum;
            if (rem >= 1 && practical[rem]) count++;
        }
    }
    return count;
}
