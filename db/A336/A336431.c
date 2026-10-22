/*
 * A336431  Number of ordered ways to write n as the sum of a practical number
 *          (A005153) and a generalized heptagonal number (A085787).
 * https://oeis.org/A336431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gota_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the number of ordered ways to write n as the sum
 * of a practical number and a generalized heptagonal number.
 *
 * The routine builds, cache‑wise, all practical numbers and all
 * generalized heptagonal numbers up to the maximum n it has ever
 * been asked to handle.  For a given n it simply counts how many
 * of the heptagonal numbers h satisfy h ≤ n and n–h is practical.
 *
 * The implementation is fully deterministic and only uses
 * standard arithmetic on the supplied integral types.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gota_realloc.h"

/* ---------- static state ---------- */
static idx_t       max_limit = 0;   /* largest n for which tables are ready */

static uint32_t   *spf          = NULL;   /* smallest prime factor of each integer */
static uint8_t    *is_practical = NULL;   /* 0/1 flag for each integer */
static size_t      spf_cap      = 0;      /* size of spf array                */

static idx_t     *hept  = NULL;   /* list of generalized heptagonal numbers ≤ max_limit */
static size_t     hept_len = 0;   /* how many heptagonal numbers are stored */
static size_t     hept_cap = 0;   /* allocated capacity for hept */

/* ---------- helpers ---------- */

/* Generates Sieve of Eratosthenes and fills spf array for 2..N */
static void build_spf(idx_t N) {
    size_t size = (size_t)N + 1;
    if (size > spf_cap) {
        free(spf);
        spf = (uint32_t *)malloc(size * sizeof(uint32_t));
        spf_cap = size;
    }
    for (size_t i = 0; i < size; ++i) spf[i] = 0;

    for (size_t i = 2; i <= (size_t)N; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= (size_t)N) {
                for (size_t j = i * i; j <= (size_t)N; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
}

/* Builds the practicalness flag array up to N */
static void build_practical(idx_t N) {
    size_t size = (size_t)N + 1;
    if (size > max_limit) {
        free(is_practical);
        is_practical = (uint8_t *)malloc(size * sizeof(uint8_t));
        max_limit = N;
    }
    is_practical[0] = 0;
    if (N >= 1) is_practical[1] = 1;
    for (idx_t m = 2; m <= N; ++m) {
        idx_t tmp = m;
        unsigned long long sum = 1;  /* σ of the product of processed prime powers */
        bool ok = true;
        while (tmp > 1) {
            uint32_t p = spf[(size_t)tmp];
            unsigned e = 0;
            while (tmp % p == 0) {
                ++e;
                tmp /= p;
            }
            if (p > sum + 1) { ok = false; break; }

            /* compute σ(p^e) = 1 + p + p^2 + ... + p^e */
            unsigned long long sigma = 1;
            unsigned long long powp  = 1;
            for (unsigned i = 0; i < e; ++i) {
                powp *= p;
                sigma += powp;
            }
            sum *= sigma;
        }
        is_practical[(size_t)m] = ok ? 1 : 0;
    }
}

/* Appends a value into the heptagonal list, enlarging it if necessary */
static void hept_append(idx_t val) {
    if (hept_len == hept_cap) {
        size_t new_cap = (hept_cap == 0) ? 64 : hept_cap << 1;
        hept = (idx_t *)gota_realloc(hept, new_cap, sizeof(idx_t));
        hept_cap = new_cap;
    }
    hept[hept_len++] = val;
}

/* Generates all generalized heptagonal numbers ≤ N */
static void build_hept(idx_t N) {
    hept_len = 0;
    if (N < 0) return;
    /* k >= 0 */
    for (idx_t k = 0;; ++k) {
        int64_t kk = (int64_t)k;
        int64_t val = kk * (5 * kk - 3) / 2;
        if (val < 0 || (idx_t)val > N) break;
        hept_append((idx_t)val);
    }
    /* negative k represented by m = |k| */
    for (idx_t m = 1;; ++m) {
        int64_t mm = (int64_t)m;
        int64_t val = mm * (5 * mm + 3) / 2;
        if (val < 0 || (idx_t)val > N) break;
        hept_append((idx_t)val);
    }
}

/* Ensures all tables are ready up to N */
static void ensure_up_to(idx_t N) {
    if (N <= max_limit) return;
    build_spf(N);
    build_practical(N);
    build_hept(N);
}

/* ---------- public function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety for n=0, though offset starts at 1 */
    ensure_up_to(n);

    seq_t count = 0;
    for (size_t i = 0; i < hept_len; ++i) {
        idx_t h = hept[i];
        if (h > n) continue;
        idx_t rem = n - h;
        if (rem <= max_limit && is_practical[(size_t)rem])
            ++count;
    }
    return count;
}
