/*
 * A325351  Heinz number of the augmented differences of the integer partition
 *          with Heinz number n.
 * https://oeis.org/A325351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Heinz number of the augmented differences of the integer
 *            partition with Heinz number n
 * Range:     u8 n <= 255, u16 n <= 9439, u32 n <= 9439, u64 n <= 9439,
 *            u128 n <= 9439, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The algorithm:
 *  1. Factor n into primes (exponents allowed).
 *  2. Map each prime to its 1‑based index in the list of primes.
 *  3. Build the partition y consisting of these indices repeated
 *     according to the exponents; sort y descending.
 *  4. Compute the augmented differences aug(y).
 *  5. The answer is the product of primes corresponding to aug(y).  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Prime sieve – one‑time initialisation                                 */
/* --------------------------------------------------------------------- */

static bool primes_ready = false;
static uint64_t *prime_list = NULL; /* 0‑based list of primes (ascending) */
static size_t prime_cnt   = 0;      /* number of primes stored          */
static const uint64_t PRIME_LIMIT = 200000;  /* enough for all tests */

/* Initialise primes once -------------------------------------------------- */
static void prepare_primes(void)
{
    if (primes_ready) return;

    /* simple sieve of Eratosthenes up to PRIME_LIMIT */
    unsigned char *is_prime = (unsigned char*)malloc(PRIME_LIMIT + 1);
    if (!is_prime) return; /* allocation failed – unlikely in tests */
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t p = 2; p * p <= PRIME_LIMIT; ++p)
        if (is_prime[p])
            for (size_t m = p * p; m <= PRIME_LIMIT; m += p)
                is_prime[m] = 0;

    /* count primes first */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    prime_cnt  = cnt;
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            prime_list[idx++] = (uint64_t)i;

    free(is_prime);
    primes_ready = true;
}

/* Binary search – return 1‑based index of prime p (assumed present). */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return mid + 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return 0; /* should never happen for valid inputs */
}

/* Comparator for qsort – descending order for uint64_t */
static int cmp_uint64_desc(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t*)a;
    const uint64_t *y = (const uint64_t*)b;
    if (*x < *y) return 1;
    if (*x > *y) return -1;
    return 0;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                 */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Special case:  n = 1 */
    if (n == (idx_t)1) return (seq_t)1;

    prepare_primes();

    /* ----------------------------------------------------------------- */
    /* 1. Factorisation of n                                            */
    /* ----------------------------------------------------------------- */
    uint64_t factors[64];
    uint64_t exps[64];
    size_t fac_cnt = 0;

    uint64_t temp = (uint64_t)n;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p != 0) continue;
        uint64_t e = 0;
        while (temp % p == 0) {
            ++e;
            temp /= p;
        }
        factors[fac_cnt] = p;
        exps[fac_cnt]    = e;
        ++fac_cnt;
    }
    if (temp > 1) { /* remaining prime factor */
        factors[fac_cnt] = temp;
        exps[fac_cnt]    = 1;
        ++fac_cnt;
    }

    /* ----------------------------------------------------------------- */
    /* 2. Build partition y (indices of primes)                          */
    /* ----------------------------------------------------------------- */
    uint64_t y[128];
    size_t y_len = 0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        size_t idx = prime_index(factors[i]);   /* 1‑based index of prime */
        for (uint64_t j = 0; j < exps[i]; ++j) {
            y[y_len++] = idx;  /* repeat according to exponent */
        }
    }

    /* The partition should be sorted non‑increasing (descending) */
    qsort(y, y_len, sizeof(uint64_t), cmp_uint64_desc);

    /* ----------------------------------------------------------------- */
    /* 3. Compute augmented differences                                 */
    /* ----------------------------------------------------------------- */
    uint64_t aug[128];
    if (y_len > 0) {
        for (size_t i = 0; i + 1 < y_len; ++i)
            aug[i] = y[i] - y[i + 1] + 1;
        aug[y_len - 1] = y[y_len - 1];
    }

    /* ----------------------------------------------------------------- */
    /* 4. Heinz number of aug(y)  -> multiply corresponding primes      */
    /* ----------------------------------------------------------------- */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < y_len; ++i) {
        size_t prime_idx = aug[i] - 1;    /* 0‑based index into prime_list */
        seq_t prime_val = (seq_t)prime_list[prime_idx];
        result = result * prime_val;
    }

    return result;
}
