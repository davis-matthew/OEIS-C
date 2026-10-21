/*
 * A272140  Numbers n such that Bernoulli number B_{n} has denominator 1590.
 * https://oeis.org/A272140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are the positive integers m such that the Bernoulli *
 *            number B_m has denominator 1590 = 2·3·5·53. * The denominator of
 *            B_m is the product of all primes p with * p‑1 | m. To have
 *            denominator 1590 we need m to be a positive * multiple of 52 and
 *            to have no other divisor d such that * d+1 is prime
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- Configuration ------------------------------------------------- */
/* Adjust MAX_K to be large enough to contain the first 10 000 terms.
 * With 52·200 000 ≈ 10 400 000 the required terms are more than covered.
 * The sieve uses an array of uint32_t of this size.
 */
#define MAX_K 200000
#define MAX_N (MAX_K * 52 + 52)

static bool initialized = false;

/* ----- Static data -------------------------------------------------------- */
static uint32_t *spf = NULL;          /* smallest prime factor for 0..MAX_N */
static seq_t *cache = NULL;           /* sequence values */
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ------------------------------------------------------------------------- */
/* Simple sieve for smallest prime factor */
static void build_spf(void)
{
    size_t i, j;
    spf = (uint32_t *)malloc((MAX_N + 1) * sizeof(uint32_t));
    if (!spf) abort();

    for (i = 0; i <= MAX_N; ++i) spf[i] = 0;
    for (i = 2; i <= MAX_N; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= MAX_N) {
                for (j = i * i; j <= MAX_N; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }
}

/* ------------------------------------------------------------------------- */
/* Recursive divisor enumeration.
 * Returns true if at least one divisor d of the original number n
 * satisfies: d != 1,2,4,52 and d+1 is prime.
 */
static bool has_bad_divisor(size_t pos, size_t n_factors,
                            const uint32_t *pr, const unsigned *ex,
                            seq_t cur, idx_t max_n)
{
    if (pos == n_factors) {
        if (cur == 1 || cur == 2 || cur == 4 || cur == 52) return false;
        idx_t val = (idx_t)cur + 1;
        if (val <= max_n && spf[val] == val) return true;
        return false;
    }
    seq_t val = cur;
    for (unsigned e = 0; e <= ex[pos]; ++e) {
        if (has_bad_divisor(pos + 1, n_factors, pr, ex, val, max_n)) return true;
        if (e < ex[pos]) val *= pr[pos];
    }
    return false;
}

/* ------------------------------------------------------------------------- */
/* Test whether n satisfies the condition for the sequence. */
static bool good_n(seq_t n)
{
    seq_t tmp = n;
    uint32_t fact_primes[16];
    unsigned fact_exp[16];
    size_t nf = 0;

    /* factorisation using the spf array */
    while (tmp != 1) {
        uint32_t p = spf[(idx_t)tmp];
        unsigned cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        fact_primes[nf] = p;
        fact_exp[nf] = cnt;
        ++nf;
    }

    /* enumerate all divisors */
    return has_bad_divisor(0, nf, fact_primes, fact_exp, 1, (idx_t)MAX_N);
}

/* ------------------------------------------------------------------------- */
/* Precomputation: build the list of allowed numbers up to MAX_N. */
static void init_cache(void)
{
    if (initialized) return;
    build_spf();

    /* initial capacity – will grow as needed */
    cache_cap = 1024;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();

    for (seq_t m = 52; m <= MAX_N; m += 52) {
        if (!good_n(m)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = m;
        }
    }

    initialized = true;
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, offset is 1 */
    init_cache();

    if (n > cache_len) {
        /* If the asked index exceeds the precalculated range, we
         * simply abort – the test harness should never request
         * such a value (the required range is covered by the
         * chosen MAX_N). */
        abort();
    }
    return cache[(size_t)n - 1];
}
