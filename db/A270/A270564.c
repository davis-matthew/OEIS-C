/*
 * A270564  Terms of A143407, sorted.
 * https://oeis.org/A270564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 28, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A270564: numbers m such that the maximal order of the
   multiplicative group modulo m equals r and there is no
   larger M with the same maximal order.  The returned terms are the
   sorted list of such m values.  The function uses a sieve to find
   the largest m for every r up to LIMIT_R, then returns the
   requested element. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* constants ----------------------------------------------------- */
#define LIMIT_R   2000000  /* search limit for r (enough for 10 000 terms) */
#define MAX_T   (LIMIT_R + 1)

/* global state ------------------------------------------------- */
static seq_t *term_arr  = NULL;   /* sorted terms */
static size_t term_cnt  = 0;      /* number of terms computed */
static bool   ready     = false;  /* all terms ready? */

/* temporary tables ------------------------------------------- */
static idx_t *spf      = NULL;   /* smallest prime factor table */
static bool   *is_prime = NULL;  /* primality flag */

/* gcd / lcm for idx_t ---------------------------------------- */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline idx_t lcm_idx(idx_t a, idx_t b)
{
    if (a == 0 || b == 0) return 0;
    return a / gcd_idx(a, b) * b;
}

/* divisor enumeration ---------------------------------------- */
static void generate_divisors(const idx_t *primes, const int *exps,
                              int depth, idx_t curr,
                              idx_t *out, size_t *cnt)
{
    if (depth == 0) {
        out[(*cnt)++] = curr;
        return;
    }
    idx_t p   = primes[depth-1];
    int    e  = exps[depth-1];
    idx_t val = curr;
    for (int i = 0; i <= e; ++i) {
        generate_divisors(primes, exps, depth-1, val, out, cnt);
        val *= p;
    }
}

/* comparison for qsort --------------------------------------- */
static int seq_cmp(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* compute maximal m for a given r ---------------------------- */
static seq_t compute_term_for_r(idx_t r)
{
    /* count 2-adic exponent of r */
    idx_t rtmp = r, e2 = 0;
    while ((rtmp & 1ULL) == 0) {
        rtmp >>= 1;
        ++e2;
    }

    /* start building m and lcm of λ-values */
    seq_t  m = 1;
    idx_t  lcm_val = 1;

    if (e2 == 0) {          /* r odd  --> use a single factor 2 */
        m = (seq_t)2;
        lcm_val = 1;
    } else {                /* r even, include largest 2-power */
        idx_t k2 = e2 + 2;          /* 2^k2 gives λ=2^e2 */
        m = (seq_t)1 << k2;        /* 2^k2 fits within idx_t */
        lcm_val = 1ULL << e2;      /* λ(2^k2) = 2^e2 */
    }

    /* factorization of r using SPF table */
    idx_t tmp = r;
    idx_t primes_f[64];
    int    exps_f[64];
    int    fcnt = 0;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes_f[fcnt] = p;
        exps_f[fcnt]   = e;
        ++fcnt;
    }

    /* generate all divisors of r */
    idx_t divisors[1024];
    size_t dcnt = 0;
    generate_divisors(primes_f, exps_f, fcnt, 1, divisors, &dcnt);

    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divisors[i];
        if (d <= 1) continue;              /* skip divisor 1 (handles 2) */
        idx_t p = d + 1;
        if (p > LIMIT_R) continue;         /* prime > table range */
        if (!is_prime[p]) continue;        /* p must be prime */

        /* maximal exponent of p so that λ(p^exp) | r */
        idx_t t    = r / d;
        int   expo = 1;
        while (t % p == 0) {
            t /= p;
            ++expo;
        }

        /* multiply m by p^expo */
        seq_t pow_val = 1;
        for (int j = 0; j < expo; ++j) pow_val *= (seq_t)p;
        m *= pow_val;

        /* λ(p^expo) = d * p^(expo-1) */
        idx_t lam = d;
        for (int j = 1; j < expo; ++j) lam *= p;

        lcm_val = lcm_idx(lcm_val, lam);
    }

    if (lcm_val == r) return m;   /* r is realizable */
    return (seq_t)0;              /* no valid m for this r */
}

/* generate all terms up to LIMIT_R --------------------------- */
static void generate_terms(void)
{
    /* build SPF and prime flag tables */
    spf = (idx_t *)malloc((LIMIT_R + 1) * sizeof(idx_t));
    if (!spf) exit(1);
    for (idx_t i = 0; i <= LIMIT_R; ++i) spf[i] = 0;

    for (idx_t i = 2; i <= LIMIT_R; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i * i <= LIMIT_R) {
                for (idx_t j = i * i; j <= LIMIT_R; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }

    is_prime = (bool *)malloc((LIMIT_R + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (idx_t i = 0; i <= LIMIT_R; ++i) {
        is_prime[i] = (i >= 2 && spf[i] == i);
    }

    /* allocate storage for terms */
    term_arr = (seq_t *)malloc(MAX_T * sizeof(seq_t));
    if (!term_arr) exit(1);
    term_cnt = 0;

    for (idx_t r = 1; r <= LIMIT_R; ++r) {
        seq_t m = compute_term_for_r(r);
        if (m != 0) {
            term_arr[term_cnt++] = m;
        }
    }

    /* sort terms */
    qsort(term_arr, term_cnt, sizeof(seq_t), seq_cmp);

    /* clean up temporary tables */
    free(spf);
    free(is_prime);
    spf = NULL;
    is_prime = NULL;

    ready = true;
}

/* public API ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!ready) generate_terms();
    if (n == 0) return (seq_t)0;
    if (n - 1 >= term_cnt) return (seq_t)0;  /* out of range */
    return term_arr[n - 1];
}
