/*
 * A346460  Square array read by downward antidiagonals in which row n lists
 *          all numbers k for which all positive integers cannot be colored
 *          with two colors without any positive integer x being the same
 *          color as n*x or k*x (for n >= 2).
 * https://oeis.org/A346460
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   gcd_uint64, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: This function computes the n‑th term of OEIS sequence A346460.
 * Each row r (r ≥ 2) lists all integers k for which ∃ positive
 * integers x, y such that r^x = k^y and x + y is odd.
 * The terms are read by downward antidiagonals:  
 *   a(2) = (2,1), a(3) = (2,2), a(4) = (3,1), a(5) = (2,3), ….
 * The function implements this definition directly.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>  /* for memset */

/* utilities available in the build */
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/gcd_uint64.h"

/* --------------------------------------------------------------------- */
/* small prime sieve for factorisation (max needed ~ 200) */
static uint32_t prime_list[200];
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_count > 0) return;
    const uint32_t N = 1000;
    bool sieve[N + 1];
    memset(sieve, true, sizeof(sieve));
    sieve[0] = sieve[1] = false;
    for (uint32_t p = 2; p * p <= N; ++p) {
        if (sieve[p]) {
            for (uint32_t q = p * p; q <= N; q += p) sieve[q] = false;
        }
    }
    for (uint32_t p = 2; p <= N; ++p) if (sieve[p]) prime_list[prime_count++] = p;
}

/* integer gcd for 64‑bit numbers */

/* factorisation of n: returns number of prime factors */
static size_t factorize(uint64_t n, uint64_t primes[], uint64_t exps[])
{
    size_t cnt = 0;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= n; ++i) {
        uint32_t p = prime_list[i];
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (n > 1) {
        primes[cnt] = n;
        exps[cnt]   = 1;
        ++cnt;
    }
    return cnt;
}

/* validity test for a pair (s, t): s+t odd and gcd(s,t)==1 */
static inline bool valid_pair(uint64_t s, uint64_t t)
{
    if ((s + t) % 2 == 0) return false;
    return gcd_uint64(s, t) == 1;
}

/* generate j‑th element of row r (row index = r, 1‑based column = j) */
static seq_t row_k(idx_t r, idx_t j)
{
    uint64_t base_r = (uint64_t)r;
    /* factorise r */
    uint64_t primes[16], exps[16];
    size_t pcnt = factorize(base_r, primes, exps);
    /* compute d = gcd of the exponents */
    uint64_t d = exps[0];
    for (size_t i = 1; i < pcnt; ++i) d = gcd_uint64(d, exps[i]);

    /* divisors of d */
    uint64_t divs[32];
    size_t dcnt = 0;
    for (uint64_t t = 1; t <= d; ++t) {
        if (d % t == 0) divs[dcnt++] = t;
    }

    /* initial s for each divisor */
    typedef struct { uint64_t s; uint64_t t; } item_t;
    item_t items[32];
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t t = divs[i];
        uint64_t s = 1;
        while (!valid_pair(s, t)) ++s;
        items[i].s = s;
        items[i].t = t;
    }

    seq_t result = 1;  /* will store current term */
    seq_t cur_val = 1; /* final return value */

    for (idx_t iter = 0; iter < j; ++iter) {
        /* find smallest fraction s/t */
        size_t min_i = 0;
        for (size_t i = 1; i < dcnt; ++i) {
            /* compare items[i].s/items[i].t vs items[min_i] */
            if (items[i].s * items[min_i].t < items[min_i].s * items[i].t) {
                min_i = i;
            }
        }
        /* compute k = r^(s/t) */
        uint64_t s = items[min_i].s;
        uint64_t t = items[min_i].t;

        /* compute prime powers */
        seq_t val = 1;
        for (size_t k = 0; k < pcnt; ++k) {
            /* exponent for prime k: a_i * s / t */
            uint64_t exp = exps[k] * s / t;  /* exact division guaranteed */
            seq_t part = pow_uint(primes[k], exp);
            val = seq_mul(val, part);
        }
        cur_val = val;  /* last value produced */

        /* advance this denominator's s to next valid value */
        do {
            items[min_i].s += 1;
        } while (!valid_pair(items[min_i].s, t));
    }
    return cur_val;
}

/* --------------------------------------------------------------------- */
/* main sequence function required by the framework */
seq_t a(idx_t n)
{
    /* n >= 2 (offset 2).  For n < 2 the behaviour is irrelevant. */
    if (n < 2) return 0;

    /* mapping from index n to coordinates (row, column) */
    idx_t N = n - 2;
    idx_t m = 0;
    /* find maximal m such that m*(m+1)/2 <= N */
    for (idx_t k = 0; k * (k + 1) / 2 <= N; ++k) m = k;
    idx_t s = m + 3;                          /* sum i + j */
    idx_t cum_before = m * (m + 1) / 2;
    idx_t offset = N - cum_before;            /* 0‑based within this sum */
    idx_t row = 2 + offset;                   /* row index i */
    idx_t col = s - row;                      /* column index j (>=1) */

    init_primes();  /* ensure primes are available */
    return row_k(row, col);
}
