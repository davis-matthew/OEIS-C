/*
 * A278647  First differences of Hypotenuse numbers A009003.
 * https://oeis.org/A278647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A009003(n+1) – A009003(n) where A009003 are the
 *            hypotenuse numbers (positive integers with a prime divisor ≡ 1
 *            (mod 4)). This implementation precomputes all hypotenuse numbers
 *            up to a fixed limit and returns the successive differences
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Maximum number up to which hypotenuse numbers are precomputed.
   For n up to 10 000 this is more than sufficient. */
static const idx_t PRECOMPUTE_LIMIT = 500000ULL;

/* Lazy static data: small‑prime factor table, list of hypotenuse numbers
   and the count of those numbers. */
static uint32_t *spf          = NULL; /* smallest prime factor for each n */
static seq_t   *hyp          = NULL; /* hypotenuse numbers, 0‑based */
static size_t   hyp_cnt      = 0;
static bool     initialized  = false;

/* Pre‑computation routine – executed once on first call. */
static void init_hyp_sequences(void)
{
    if (initialized)
        return;

    /* 1. Allocate and initialise the SPF array. */
    spf = (uint32_t*)malloc((PRECOMPUTE_LIMIT + 1) * sizeof(uint32_t));
    if (!spf)
        return;  /* allocation failed – unlikely outcome */

    for (idx_t i = 0; i <= PRECOMPUTE_LIMIT; ++i)
        spf[i] = (uint32_t)i;

    /* Sieve for smallest prime factor. */
    idx_t sqrt_limit = 0;
    while ((sqrt_limit + 1) * (sqrt_limit + 1) <= PRECOMPUTE_LIMIT)
        ++sqrt_limit;

    for (idx_t i = 2; i <= sqrt_limit; ++i) {
        if (spf[i] == (uint32_t)i) {          /* i is prime */
            for (idx_t j = i * i; j <= PRECOMPUTE_LIMIT; j += i) {
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
            }
        }
    }

    /* 2. Allocate space for hypotenuse numbers.  The real count is small
       compared with PRECOMPUTE_LIMIT; we allocate the full limit to avoid
       reallocations. */
    hyp = (seq_t*)malloc(PRECOMPUTE_LIMIT * sizeof(seq_t));
    if (!hyp) {
        free(spf);
        return;
    }
    hyp_cnt = 0;

    /* 3. Scan all integers and collect those with a prime factor
       congruent 1 modulo 4. */
    for (idx_t m = 1; m <= PRECOMPUTE_LIMIT; ++m) {
        idx_t temp = m;
        bool has14 = false;

        while (temp > 1) {
            uint32_t p = spf[(size_t)temp];
            if ((p & 3) == 1) {          /* p ≡ 1 (mod 4) */
                has14 = true;
                break;
            }
            /* Skip repeated factors. */
            while (temp % p == 0)
                temp /= p;
        }

        if (has14)
            hyp[hyp_cnt++] = (seq_t)m;
    }

    /* 4. Clean up and mark done. */
    free(spf);
    initialized = true;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined below the offset */

    init_hyp_sequences();

    /* The function is only required for n ≥ 1.  If, due to an unexpected
       request, n exceeds the number of precomputed hypotenuse numbers,
       return 0 as a safe default. */
    if ((size_t)n >= hyp_cnt)
        return (seq_t)0;

    /* a(n) = H(n+1) – H(n), where H is the hypotenuse sequence
       and hyp[] stores H(1), H(2), …  (0‑based). */
    return hyp[(size_t)n] - hyp[(size_t)n - 1];
}
