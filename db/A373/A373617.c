/*
 * A373617  Positive integers that cannot be written as a sum of a practical
 *          number and a 14-gonal number.
 * https://oeis.org/A373617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 77, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search/sieve implementation:
   a(n) is the n‑th positive integer that cannot be written as a sum of
   a practical number and a 14‑gonal number (0 is a legal 14‑gonal number). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static bool init_done = false;
static idx_t L_current = 0;               /* current sieve limit               */

static idx_t *spf = NULL;                 /* smallest prime factor array        */
static size_t spf_cap = 0;

static idx_t *practical = NULL;           /* list of practical numbers          */
static size_t practical_cnt = 0;
static size_t practical_cap = 0;

static idx_t *gonal = NULL;               /* list of 14‑gonal numbers (incl. 0)*/
static size_t gonal_cnt = 0;
static size_t gonal_cap = 0;

static bool *representable = NULL;        /* representable[0..limit]            */
static size_t repr_cap = 0;

static idx_t *missing = NULL;             /* list of non‑representable numbers */
static size_t missing_cnt = 0;
static size_t missing_cap = 0;

/* ---------- sieve of smallest prime factors (linear sieve) ---------- */
static void compute_spf(idx_t limit)
{
    if (spf) free(spf);
    spf_cap = (size_t)(limit + 1);
    spf = (idx_t *)malloc(spf_cap * sizeof(idx_t));
    memset(spf, 0, spf_cap * sizeof(idx_t));
    spf[0] = 0;
    spf[1] = 1;

    idx_t *pr = (idx_t *)malloc(limit * sizeof(idx_t));
    size_t pr_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            pr[pr_cnt++] = i;
        }
        for (size_t j = 0; j < pr_cnt && pr[j] <= spf[i] &&
             (idx_t)i * pr[j] <= limit; ++j) {
            spf[(size_t)i * pr[j]] = pr[j];
        }
    }
    free(pr);
}

/* ---------- test practicality of a number using known criteria ---------- */
static bool is_practical(idx_t n)
{
    if (n == 1) return true;
    idx_t tmp = n;
    bool first = true;
    unsigned long long sigma_cur = 1;       /* product of σ for processed primes */

    while (tmp > 1) {
        idx_t p = spf[tmp];
        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }

        if (first) {
            if (p != 2) return false;
            first = false;
        } else {
            if ((unsigned long long)p > sigma_cur + 1ULL)
                return false;
        }

        /* compute σ(p^exp) = 1 + p + … + p^exp */
        unsigned long long pow = 1;
        unsigned long long sum = 1;
        for (int i = 0; i < exp; ++i) {
            pow *= (unsigned long long)p;
            sum += pow;
        }
        sigma_cur *= sum;
    }
    return true;
}

/* ---------- rebuild all data up to a given limit -------------------------------- */
static void rebuild(idx_t limit)
{
    /* free old data except spf (will be recreated) */
    free(practical);
    free(gonal);
    free(representable);
    free(missing);

    /* 1. smallest prime factors up to limit */
    compute_spf(limit);
    L_current = limit;

    /* 2. practical numbers up to limit */
    practical_cap = 256;
    practical = (idx_t *)malloc(practical_cap * sizeof(idx_t));
    practical_cnt = 0;
    practical[practical_cnt++] = 1;   /* 1 is practical */

    for (idx_t i = 2; i <= limit; ++i) {
        if (is_practical(i)) {
            if (practical_cnt == practical_cap) {
                practical_cap <<= 1;
                practical = (idx_t *)realloc(practical,
                                             practical_cap * sizeof(idx_t));
            }
            practical[practical_cnt++] = i;
        }
    }

    /* 3. 14‑gonal numbers (include 0) */
    gonal_cap = 64;
    gonal = (idx_t *)malloc(gonal_cap * sizeof(idx_t));
    gonal_cnt = 0;
    gonal[gonal_cnt++] = 0;          /* n = 0 */
    for (idx_t k = 1;; ++k) {
        idx_t val = 6 * k * k - 5 * k;
        if (val > limit) break;
        if (gonal_cnt == gonal_cap) {
            gonal_cap <<= 1;
            gonal = (idx_t *)realloc(gonal,
                                     gonal_cap * sizeof(idx_t));
        }
        gonal[gonal_cnt++] = val;
    }

    /* 4. mark representable sums */
    repr_cap = (size_t)(limit + 1);
    representable = (bool *)calloc(repr_cap, sizeof(bool));
    for (size_t i = 0; i < practical_cnt; ++i) {
        idx_t p = practical[i];
        for (size_t j = 0; j < gonal_cnt; ++j) {
            idx_t sum = p + gonal[j];
            if (sum <= limit)
                representable[(size_t)sum] = true;
        }
    }

    /* 5. collect missing (non‑representable) numbers */
    missing_cap = 1024;
    missing = (idx_t *)malloc(missing_cap * sizeof(idx_t));
    missing_cnt = 0;
    for (idx_t val = 1; val <= limit; ++val) {
        if (!representable[(size_t)val]) {
            if (missing_cnt == missing_cap) {
                missing_cap <<= 1;
                missing = (idx_t *)realloc(missing,
                                           missing_cap * sizeof(idx_t));
            }
            missing[missing_cnt++] = val;
        }
    }
}

/* ---------- public function ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined position, return 0 */

    if (!init_done) {
        rebuild(200000);         /* initial sieve limit */
        init_done = true;
    }

    /* increase limit until we have at least n missing values */
    while (missing_cnt < (size_t)n) {
        idx_t new_limit = L_current * 2;
        if (new_limit <= L_current) new_limit = L_current + 1; /* safety */
        rebuild(new_limit);
    }
    return (seq_t)missing[n - 1];
}
