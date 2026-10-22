/*
 * A377181  Rectangular array, by antidiagonals: (row 1) = r(1) = A002808
 *          (composite numbers); (row n) = r(n) = A002808(r(n-1)) for n>=1.
 * https://oeis.org/A377181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A377181 – Rectangular array by antidiagonals
    The sequence is defined by the array rows

          r(1) = composite numbers A002808
          r(n) = composite(r(n‑1)) for n ≥ 2

    The terms are read by antidiagonals of this array.
    For an index n ≥ 1 the coordinates (i,j) satisfy

          n = 1 + 2 + … + (d‑1) + k
          i = k
          j = d – k + 1       (1 ≤ k ≤ d)

    where d is the smallest integer with d(d+1)/2 ≥ n.
    The value a(n) is obtained by applying the composite‑function
    f(x) = the x‑th composite number f‑times, i.e.

          a(n) = f^i(j)     (f applied i times to j)

    The implementation below precomputes primes up to a fixed bound,
    uses the prime‑count array to obtain the x‑th composite number
    via binary search, and then evaluates the required power of f.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Public interface required by the build system */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/*  Prime sieve and prefix prime count                                 */
/* ------------------------------------------------------------------ */

#define SIEVE_MAX 25000000U    /* 25 M primes – enough for n ≤ 10 000 */

static unsigned char *is_prime   = NULL;   /* 0/1 flag   */
static uint32_t      *prime_cnt  = NULL;   /* π(x)       */
static bool          sieve_inited = false;

/* Initialise the sieve and prefix array */
static void init_sieve(void)
{
    if (sieve_inited) return;
    const idx_t N = SIEVE_MAX;
    is_prime = (unsigned char*)malloc((N + 1) * sizeof(unsigned char));
    if (!is_prime) abort();
    prime_cnt = (uint32_t*)malloc((N + 1) * sizeof(uint32_t));
    if (!prime_cnt) abort();

    for (idx_t i = 0; i <= N; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t p = 2; p * p <= N; ++p) if (is_prime[p])
        for (idx_t q = p * p; q <= N; q += p) is_prime[q] = 0;

    prime_cnt[0] = 0;
    for (idx_t i = 1; i <= N; ++i)
        prime_cnt[i] = prime_cnt[i - 1] + (uint32_t)is_prime[i];

    sieve_inited = true;
}

/* Return the n‑th composite number (n ≥ 1).  n is a 64‑bit integer. */
static seq_t composite(seq_t n)
{
    /* n should fit into the sieve range, otherwise the bound must be raised */
    idx_t idx = (idx_t)n;          /* index request for the composite table   */
    idx_t low  = idx + 1;          /* composite numbers start at 4 (index 1)   */
    idx_t high = SIEVE_MAX;        /* upper bound                                    */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t comp_cnt = (idx_t)mid - prime_cnt[mid] - 1;  /* composites ≤ mid */
        if (comp_cnt >= n) high = mid;
        else               low  = mid + 1;
    }
    return (seq_t)low;
}

/* ------------------------------------------------------------------ */
/*  Sequence evaluation                                               */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* initialise sieve once */
    if (!sieve_inited) init_sieve();

    /* ---- Antidiagonal coordinates ---- */
    idx_t d = (idx_t)((sqrt((long double)8.0L * n + 1.0L) - 1.0L) / 2.0L);
    /* correct potential rounding errors */
    while (d * (d + 1) / 2 < n) ++d;
    while (d > 0 && (d - 1) * d / 2 >= n) --d;

    idx_t prev_diag = (d - 1) * d / 2;   /* elements before this diagonal   */
    idx_t k        = n - prev_diag;      /* position inside the diagonal   */
    idx_t i        = k;                  /* row index (1‑based)             */
    idx_t j        = d - k + 1;          /* column index (1‑based)          */

    /* ---- Apply the composite map i times to j ---- */
    seq_t x = j;
    for (idx_t t = 0; t < i; ++t) {
        x = composite(x);
    }
    return x;
}
