/*
 * A328009  Irregular array read by rows in which row n lists the divisors of
 *          the n-th term of the sequence of amicable pairs (A259180).
 * https://oeis.org/A328009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth divisor in the flattened list of
   sorted divisors of successive amicable numbers from A259180 (row-wise). */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* The build supplies seq_t and idx_t */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Global state for lazy generation */
static uint32_t *spf       = NULL;           /* smallest prime factor array */
static const uint32_t LIMIT  = 2000000;     /* limit for amicable search */
static uint64_t cur_n      = 2;             /* next integer to examine */
static seq_t *seq_arr      = NULL;          /* concatenated divisor list */
static size_t seq_len      = 0;
static size_t seq_cap      = 0;

/* ------------------------------------------------------------------ */
/* Comparator for qsort over seq_t */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    return (*x < *y) ? -1 : (*x > *y);
}

/* ------------------------------------------------------------------ */
/* Build smallest prime factor array up to LIMIT */
static void build_spf(void)
{
    spf = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    if (!spf) exit(1);
    spf[1] = 1;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                    if (spf[(uint32_t)j] == 0) spf[(uint32_t)j] = i;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Compute sigma (sum of all divisors, including the number) */
static uint64_t sigma_of(uint32_t n)
{
    uint64_t sum = 1;
    uint32_t temp = n;
    while (temp > 1) {
        uint32_t p = spf[temp];
        uint32_t e   = 0;
        while (temp % p == 0) { temp /= p; ++e; }
        uint64_t term = 1;
        uint64_t power = 1;
        for (uint32_t k = 0; k < e; ++k) {
            power *= p;
            term += power;
        }
        sum *= term;
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/* Compute proper divisor sum (sigma minus n) */
static uint32_t proper_sum(uint32_t n)
{
    return (uint32_t)(sigma_of(n) - n);
}

/* ------------------------------------------------------------------ */
/* Generate sorted divisors of x, return array and count */
static void generate_divisors(uint32_t x, seq_t **out, size_t *cnt_out)
{
    /* factorization */
    uint32_t primes[32];
    uint32_t exps[32];
    size_t pcnt = 0;
    uint32_t temp = x;
    while (temp > 1) {
        uint32_t p = spf[temp];
        uint32_t e = 0;
        while (temp % p == 0) { temp /= p; ++e; }
        primes[pcnt] = p;
        exps[pcnt]  = e;
        ++pcnt;
    }
    /* total number of divisors */
    size_t dcnt = 1;
    for (size_t i = 0; i < pcnt; ++i) dcnt *= (size_t)(exps[i] + 1);
    seq_t *divs = (seq_t *)malloc(dcnt * sizeof(seq_t));
    if (!divs) exit(1);
    size_t sz = 1;
    divs[0] = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint32_t p = primes[i];
        uint32_t e = exps[i];
        size_t cur_sz = sz;
        uint64_t p_pow = 1;
        for (uint32_t k = 1; k <= e; ++k) {
            p_pow *= p;
            for (size_t j = 0; j < cur_sz; ++j) {
                divs[sz++] = (seq_t)divs[j] * (seq_t)p_pow;
            }
        }
    }
    /* sort ascending */
    qsort(divs, dcnt, sizeof(seq_t), cmp_seq_t);
    *out      = divs;
    *cnt_out  = dcnt;
}

/* ------------------------------------------------------------------ */
/* Ensure seq_arr has capacity for at least needed */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1024;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);
    seq_arr = new_arr;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Append terms to global sequence */
static void add_terms(const seq_t *terms, size_t count)
{
    size_t new_len = seq_len + count;
    ensure_seq_cap(new_len);
    memcpy(seq_arr + seq_len, terms, count * sizeof(seq_t));
    seq_len = new_len;
}

/* ------------------------------------------------------------------ */
/* Generate amicable numbers and append their divisors until we reach
   at least needed_len sequence terms. Called lazily from a(). */
static void generate_sequence_upto(size_t needed_len)
{
    if (!spf) build_spf();    /* build once */

    while (seq_len < needed_len && cur_n <= LIMIT) {
        uint32_t n = (uint32_t)cur_n;
        uint32_t m = proper_sum(n);
        if (m != n && m <= LIMIT && proper_sum(m) == n) {
            seq_t *divs;
            size_t dcnt;
            generate_divisors(n, &divs, &dcnt);
            add_terms(divs, dcnt);
            free(divs);
        }
        ++cur_n;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n < 1 */
    size_t idx = (size_t)n - 1;

    if (seq_arr == NULL)                          /* first call */
        seq_len = 0;

    if (idx >= seq_len)
        generate_sequence_upto(idx + 1);  /* generate until we have idx+1 terms */

    return (idx < seq_len) ? seq_arr[idx] : (seq_t)0;
}
