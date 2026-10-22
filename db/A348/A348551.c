/*
 * A348551  Heinz numbers of integer partitions whose mean is not an integer.
 * https://oeis.org/A348551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   No simple closed form – it enumerates integers and tests whether
 *            the corresponding partition has non‑integral mean
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* sieve parameters */
#define PRIMELIMIT 200000U

/* global prime array */
static uint64_t *primes = NULL;
static size_t   primesCnt = 0;

/* initialise the sieve and prime array once */
static void init_primes(void)
{
    if (primes != NULL)
        return;                               /* already done */

    unsigned char *isComposite = (unsigned char*)malloc(PRIMELIMIT + 1);
    memset(isComposite, 0, PRIMELIMIT + 1);

    for (uint64_t i = 2; i * i <= PRIMELIMIT; ++i) {
        if (!isComposite[i]) {
            for (uint64_t j = i * i; j <= PRIMELIMIT; j += i)
                isComposite[j] = 1;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= PRIMELIMIT; ++i)
        if (!isComposite[i])
            ++cnt;

    primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    primesCnt = cnt;

    size_t idx = 0;
    for (uint64_t i = 2; i <= PRIMELIMIT; ++i)
        if (!isComposite[i])
            primes[idx++] = i;

    free(isComposite);
}

/* 1‑based index of a prime.  Assumes p <= PRIMELIMIT. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = primesCnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo + 1;                       /* convert to 1‑based index */
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_primes();

    idx_t target = n;
    idx_t found  = 0;
    uint64_t m   = 1;                    /* candidate integer */

    while (true) {
        bool ok;

        if (m == 1) {
            /* empty partition: mean undefined → accepted */
            ok = true;
        } else {
            uint64_t temp = m;
            uint64_t len   = 0;          /* total number of parts (exponent sum) */
            uint64_t sumIdx = 0;         /* sum of prime indices, counting multiplicity */

            /* trial division by precomputed primes */
            for (size_t i = 0; i < primesCnt && primes[i] * primes[i] <= temp; ++i) {
                uint64_t p = primes[i];
                if (temp % p == 0) {
                    uint64_t exp = 0;
                    while (temp % p == 0) {
                        temp /= p;
                        ++exp;
                    }
                    len   += exp;
                    sumIdx += exp * (i + 1);    /* i+1 : 1‑based prime index */
                }
            }

            if (temp > 1) {                 /* remaining factor is prime > sqrt(m) */
                size_t idx = prime_index(temp); /* temp <= PRIMELIMIT for n up to ~1e4 */
                len   += 1;
                sumIdx += idx;
            }

            /* mean = sumIdx / len; non‑integral if remainder != 0 */
            ok = (len > 0) && ((sumIdx % len) != 0);
        }

        if (ok) {
            ++found;
            if (found == target)
                return (seq_t)m;
        }
        ++m;
    }
}
