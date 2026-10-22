/*
 * A325799  Sum of the prime indices of n minus the number of distinct
 *          positive subset-sums of the prime indices of n.
 * https://oeis.org/A325799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A056239(n) - A304793(n) A056239(n) = sum of prime indices
 *            of n A304793(n) = number of distinct positive subset‑sums of
 *            those indices Implementation: sieve up to n, factor n, then
 *            subset‑sum DP with a bitset
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache of primes and their 1‑based index */
static uint32_t *primes = NULL;          /* primes[0 .. primes_cnt-1] */
static uint16_t *prime_index = NULL;     /* prime_index[p] = 1‑based index of prime p */
static size_t primes_cnt = 0;
static idx_t sieve_limit = 0;

/* Build sieve up to 'limit' (inclusive). */
static void build_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* free old memory if any */
    free(primes);
    free(prime_index);

    prime_index = (uint16_t *)calloc((size_t)(limit + 1), sizeof(uint16_t));
    primes      = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));   /* over‑allocate, real size primes_cnt */

    unsigned char *is_comp = (unsigned char *)calloc((size_t)(limit + 1), sizeof(unsigned char));
    primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt] = (uint32_t)i;
            prime_index[i] = (uint16_t)(primes_cnt + 1); /* 1‑based index */
            ++primes_cnt;

            if (i * i <= limit)
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
        }
    }

    free(is_comp);
    sieve_limit = limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    if (n > sieve_limit)
        build_sieve(n);

    /* Factor n, recording prime indices and multiplicities */
    idx_t tmp = n;
    uint16_t indices[20];
    uint16_t exps[20];
    size_t fac = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > tmp)
            break;
        if (tmp % p == 0) {
            uint16_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            indices[fac] = prime_index[p];
            exps[fac]    = cnt;
            ++fac;
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        indices[fac] = prime_index[(uint32_t)tmp];
        exps[fac]    = 1;
        ++fac;
    }

    seq_t sum_indices = 0;
    for (size_t i = 0; i < fac; ++i)
        sum_indices += (seq_t)indices[i] * (seq_t)exps[i];

    /* Subset‑sum DP */
    size_t sumS = (size_t)sum_indices;
    if (sumS == 0)
        return 0;

    unsigned char *bits = (unsigned char *)calloc(sumS + 1, sizeof(unsigned char));
    if (!bits)
        return sum_indices; /* fallback */

    bits[0] = 1;
    for (size_t k = 0; k < fac; ++k) {
        uint16_t idx = indices[k];
        uint16_t times = exps[k];
        for (uint16_t rep = 0; rep < times; ++rep) {
            for (int64_t s = (int64_t)sumS; s >= idx; --s) {
                if (bits[s - idx])
                    bits[s] = 1;
            }
        }
    }

    seq_t distinct_sums = 0;
    for (size_t s = 1; s <= sumS; ++s)
        if (bits[s])
            ++distinct_sums;

    free(bits);
    return sum_indices - distinct_sums;
}
