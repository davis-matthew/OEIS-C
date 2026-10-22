/*
 * A346701  Heinz number of the odd bisection (odd-indexed parts) of the
 *          integer partition with Heinz number n.
 * https://oeis.org/A346701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) is computed by factorising n, interpreting the factor
 *            exponents as multiplicities of parts in the corresponding
 *            partition, taking only the odd‑indexed parts, and multiplying
 *            the primes of those parts. This follows directly from the
 *            definition of the Heinz number and odd bisection
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static uint64_t *prime_list = NULL;    /* primes up to 10000, 1‑based index */
static size_t prime_count = 0;

/* Initialise the prime table once (sieve of Eratosthenes). */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;
    const uint64_t LIMIT = 10000;
    size_t sz = LIMIT + 1;
    unsigned char *sieve = (unsigned char *)calloc(sz, 1);
    if (!sieve)
        return;                     /* out of memory – unlikely */
    sieve[0] = sieve[1] = 1;
    for (uint64_t i = 2; i * i <= LIMIT; ++i)
        if (!sieve[i])
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 1;
    /* Count primes */
    prime_count = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            ++prime_count;
    prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            prime_list[idx++] = i;
    free(sieve);
}

/* Return 1‑based index of prime p; 0 if p is not prime. */
static size_t prime_index_of(uint64_t p)
{
    init_primes();
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t val = prime_list[mid];
        if (val == p)
            return mid + 1;            /* 1‑based */
        if (val < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;   /* should not happen for valid input */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for offset 1 but safe */

    init_primes();

    /* Factorise n into primes and exponents.  The number of distinct
       prime factors is very small for n <= 10000. */
    uint64_t primes_buf[64];
    int    exps_buf[64];
    size_t factor_cnt = 0;
    uint64_t tmp = (uint64_t)n;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > tmp)
            break;
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            primes_buf[factor_cnt] = p;
            exps_buf[factor_cnt] = exp;
            ++factor_cnt;
        }
    }
    if (tmp > 1) {
        primes_buf[factor_cnt] = tmp;
        exps_buf[factor_cnt] = 1;
        ++factor_cnt;
    }

    /* Build (index, exponent) pairs: index = prime number index. */
    size_t idx_buf[64];
    size_t cnt = 0;
    for (size_t i = 0; i < factor_cnt; ++i) {
        size_t j = prime_index_of(primes_buf[i]);   /* 1‑based part size */
        if (j == 0)   /* should not occur */
            continue;
        idx_buf[cnt] = j;
        exps_buf[cnt] = exps_buf[i];
        ++cnt;
    }

    /* Sort pairs by part index descending so that partition is non‑increasing. */
    for (size_t i = 0; i < cnt; ++i)
        for (size_t j = i + 1; j < cnt; ++j)
            if (idx_buf[j] > idx_buf[i]) {
                size_t tmpi = idx_buf[i];
                idx_buf[i] = idx_buf[j];
                idx_buf[j] = tmpi;
                int tmpe = exps_buf[i];
                exps_buf[i] = exps_buf[j];
                exps_buf[j] = tmpe;
            }

    /* Compute product of primes for odd‑indexed parts. */
    seq_t result = 1;
    uint64_t pos = 1;          /* 1‑based position in partition */

    for (size_t k = 0; k < cnt; ++k) {
        uint64_t part_index = idx_buf[k];          /* part size */
        uint64_t exp      = (uint64_t)exps_buf[k]; /* multiplicity of this part */

        uint64_t odd_cnt = 0;
        if (pos & 1ULL)                /* starting on odd position */
            odd_cnt = (exp + 1ULL) >> 1;      /* ceil(exp/2) */
        else
            odd_cnt = exp >> 1;               /* floor(exp/2) */

        if (odd_cnt > 0) {
            uint64_t prime_of_part = prime_list[part_index - 1]; /* j‑th prime */
            for (uint64_t t = 0; t < odd_cnt; ++t)
                result *= (seq_t)prime_of_part;
        }

        pos += exp;              /* advance past this block of parts */
    }

    return result;
}
