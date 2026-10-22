/*
 * A373646  Positive integers that cannot be written as a sum of a practical
 *          number and an 18-gonal number.
 * https://oeis.org/A373646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 680, u32 n <= 1020, u64 n <= 1020,
 *            u128 n <= 1020, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373646
 *
 * A(n) is the n‑th positive integer that cannot be written as the sum of a
 * practical number and an 18‑gonal number (including 0 as an 18‑gonal
 * number, i.e. k ≥ 0).  The sequence starts with 10, 11, 14, 15, ….
 * The implementation precomputes all such integers up to a limit that
 * covers the conjectured largest member (8 541 224) and then returns the
 * n‑th term in O(1).  The precomputation uses a sieve of size
 * LIMIT+1; it is performed on the first call.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 9500000ULL          /* > 8 541 224, sufficient for known terms */

/* ---------- static caches ---------- */
static bool initialized = false;

/* smallest prime factor array (uint32_t) */
static uint32_t *spf = NULL;

/* bit array marking representable sums (uint8_t) */
static uint8_t *representable = NULL;

/* sorted list of the required integers (seq_t) */
static seq_t *table = NULL;
static size_t table_len = 0;

/* ---------- helper: build spf array ---------- */
static void build_spf(void)
{
    spf = (uint32_t*)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
    if (!spf) abort();

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    if (!spf[j]) spf[j] = (uint32_t)i;
            }
        }
    }
}

/* ---------- helper: test practicality using the spf array ---------- */
static bool is_practical(uint32_t n)
{
    if (n == 1) return true;
    if (n & 1) return false;              /* practical numbers >1 are even */

    uint64_t remaining = n;
    uint64_t sigma_prev = 1;
    bool first = true;

    while (remaining > 1) {
        uint32_t p = spf[remaining];
        int e = 0;
        while (remaining % p == 0) {
            remaining /= p;
            ++e;
        }

        /* sigma(p^e) = (p^(e+1)-1)/(p-1) */
        uint64_t pow = 1;
        for (int i = 0; i < e + 1; ++i) pow *= p;
        uint64_t sigma = (pow - 1) / (p - 1);

        if (first) {
            if (p != 2) return false;      /* first prime must be 2 */
            sigma_prev = sigma;
            first = false;
        } else {
            if ((uint64_t)p > sigma_prev + 1) return false;
            sigma_prev *= sigma;
        }
    }
    return true;
}

/* ---------- pre‑computation routine ---------- */
static void precompute(void)
{
    build_spf();

    /* gather practical numbers in an array */
    uint32_t *practical = NULL;
    size_t pr_cap = 1024, pr_len = 0;
    practical = (uint32_t*)malloc(pr_cap * sizeof(uint32_t));
    if (!practical) abort();

    for (uint32_t n = 1; n <= LIMIT; ++n) {
        if (is_practical(n)) {
            if (pr_len == pr_cap) {
                pr_cap <<= 1;
                practical = (uint32_t*)realloc(practical, pr_cap * sizeof(uint32_t));
                if (!practical) abort();
            }
            practical[pr_len++] = n;
        }
    }

    /* gather 18‑gonal numbers (k ≥ 0) */
    uint32_t *gonal = NULL;
    size_t gn_cap = 128, gn_len = 0;
    gonal = (uint32_t*)malloc(gn_cap * sizeof(uint32_t));
    if (!gonal) abort();

    for (uint32_t k = 0;; ++k) {
        uint64_t val = 8ULL * k * k - 7ULL * k;   /* 8k² – 7k */
        if (val > LIMIT) break;
        if (gn_len == gn_cap) {
            gn_cap <<= 1;
            gonal = (uint32_t*)realloc(gonal, gn_cap * sizeof(uint32_t));
            if (!gonal) abort();
        }
        gonal[gn_len++] = (uint32_t)val;
    }

    /* mark representable sums p + g (p practical, g 18‑gonal) */
    representable = (uint8_t*)calloc((size_t)LIMIT + 1, sizeof(uint8_t));
    if (!representable) abort();

    for (size_t i = 0; i < pr_len; ++i) {
        uint32_t p = practical[i];
        for (size_t j = 0; j < gn_len; ++j) {
            uint32_t g = gonal[j];
            uint64_t sum = (uint64_t)p + (uint64_t)g;
            if (sum > LIMIT) break;
            representable[(idx_t)sum] = 1;
        }
    }

    /* build table of numbers NOT representable */
    table = (seq_t*)malloc((size_t)LIMIT * sizeof(seq_t));
    if (!table) abort();

    for (idx_t x = 1; x <= LIMIT; ++x) {
        if (!representable[(idx_t)x]) {
            table[table_len++] = (seq_t)x;
        }
    }

    /* shrink table to actual size */
    if (table_len > 0) {
        seq_t *tmp = (seq_t*)realloc(table, table_len * sizeof(seq_t));
        if (tmp) table = tmp;
    } else {
        free(table);
        table = NULL;
    }

    /* free temporaries */
    free(practical);
    free(gonal);
    free(representable);
    free(spf);

    initialized = true;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n<1 */
    if (!initialized) precompute();
    if ((size_t)n > table_len) return 0;/* exceeds pre‑computed range */
    return table[(size_t)n - 1];
}
