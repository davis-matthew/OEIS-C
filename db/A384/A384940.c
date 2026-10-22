/*
 * A384940  Odd semiprimes interleaved with even semiprimes.
 * https://oeis.org/A384940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(2*k-1) = A046315(k) (k‑th odd semiprime), * a(2*k) =
 *            A100484(k) (k‑th even semiprime)
 * Range:     u8 n <= 63, u16 n <= 7025, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Precomputation of odd and even semiprimes up to the required size  */
/* ------------------------------------------------------------------ */

#define MAX_PER_TYPE 6000   /* buffer for odd/even semiprimes                */
#define PRECOMP_LIMIT 200000   /* upper bound to collect enough entries        */

static bool          initialized = false;
static seq_t        *odd_semiprimes  = NULL;
static seq_t        *even_semiprimes = NULL;
static size_t        odd_count  = 0;
static size_t        even_count = 0;
static uint32_t     *spf         = NULL;   /* smallest prime factor sieve */

/* Count the number of prime factors of n using the spf array.
   Stops early if the count exceeds 2. */
static inline int count_factors(uint64_t n)
{
    int c = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        ++c;
        n /= p;
        if (c > 2) return c;
    }
    return c;
}

/* Build the smallest‑prime‑factor table up to LIMIT.  */
/*  LIMIT must be at least as large as the largest number we test.          */
static void build_spf(uint32_t limit)
{
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    for (uint32_t i = 0; i <= limit; ++i) spf[i] = i;
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (spf[i] == i) {          /* i is prime */
            for (uint32_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
}

/* Precompute odd and even semiprimes into static arrays.  */
static void init_sequence(void)
{
    if (initialized) return;

    build_spf(PRECOMP_LIMIT);
    odd_semiprimes  = (seq_t *)malloc(MAX_PER_TYPE * sizeof(seq_t));
    even_semiprimes = (seq_t *)malloc(MAX_PER_TYPE * sizeof(seq_t));

    for (uint32_t m = 3; m <= PRECOMP_LIMIT; ++m) {
        if (count_factors(m) == 2) {     /* m is a semiprime */
            if ((m & 1u) == 0) {         /* even semiprime */
                if (even_count < MAX_PER_TYPE)
                    even_semiprimes[even_count++] = (seq_t)m;
            } else {                     /* odd semiprime */
                if (odd_count < MAX_PER_TYPE)
                    odd_semiprimes[odd_count++] = (seq_t)m;
            }
        }
        if (odd_count >= MAX_PER_TYPE && even_count >= MAX_PER_TYPE)
            break;
    }
    initialized = true;
}

/* ------------------------------------------------------------------ */
/*                     Public function: seq_t a(idx_t n)                */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for index < 1 */

    if (!initialized) init_sequence();

    if ((n & 1u) == 0) {                /* even index --> even semiprime */
        idx_t k = n / 2;                /* 1‑based position in the even list */
        if ((size_t)k - 1 >= even_count) return (seq_t)0;   /* safety check */
        return even_semiprimes[(size_t)k - 1];
    } else {                            /* odd index --> odd semiprime */
        idx_t k = (n + 1) / 2;          /* 1‑based position in the odd list */
        if ((size_t)k - 1 >= odd_count) return (seq_t)0;    /* safety check */
        return odd_semiprimes[(size_t)k - 1];
    }
}
