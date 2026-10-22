/*
 * A376612  Triangle read by rows: T(n,k) is the largest divisor of
 *          binomial(n,k) which is less than n, with 2 <= k <= n.
 * https://oeis.org/A376612
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look, tabl
 * Formula:   a(n) is the largest divisor of binomial(r,k) that is < k,
 * Range:     u8 n <= 5051, u16 n <= 5051, u32 n <= 5051, u64 n <= 5051,
 *            u128 n <= 5051, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// where r and k are determined by unfolding the triangle read by rows.

// n >= 2, a(2) is the first term; a(n) for n < 2 is defined as 0.

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* The build supplies seq_t and idx_t.  Only use + - * / % and
   integer types.  seq_t could be uint8_t, uint16_t, uint32_t, uint64_t,
   unsigned __int128, or an arbitrary-precision type. */

/* Small prime cache (enough for n <= 200000). */
static uint32_t primes[200000];
static size_t primeCnt = 0;
static uint32_t lastChecked = 1;

/* Expand prime list up to 'limit' (inclusive). */
static void ensure_primes_upto(uint32_t limit)
{
    if (limit <= lastChecked)
        return;
    for (uint32_t i = lastChecked + 1; i <= limit; ++i) {
        bool isPrime = true;
        for (size_t j = 0; j < primeCnt && primes[j] * primes[j] <= i; ++j) {
            if (i % primes[j] == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            primes[primeCnt++] = i;
        }
    }
    lastChecked = limit;
}

/* Exponent of prime p in n! (Legendre). */
static int exp_fact(idx_t n, uint32_t p)
{
    int e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* Return largest divisor of C(r,k) that is < k. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* Convert index to row/column by unfolding the triangle. */
    idx_t s = n - 2;          /* zero‑based offset in the sequence */
    idx_t r = 2;              /* current row (starts at 2) */
    idx_t cum = 0;            /* total terms before current row */

    while (cum + (r - 1) <= s) {  /* find correct row */
        cum += (r - 1);
        ++r;
    }

    idx_t pos = s - cum;      /* zero‑based position within the row */
    idx_t k = pos + 2;        /* column index (2 <= k <= r) */

    /* Prepare primes up to 'r' for factorisation. */
    ensure_primes_upto((uint32_t)r);

    /* Compute prime exponents of C(r,k). */
    int *expBinom = (int *)malloc(sizeof(int) * primeCnt);
    if (!expBinom)
        return (seq_t)0;      /* allocation failure, should not happen */

    for (size_t i = 0; i < primeCnt; ++i) {
        uint32_t p = primes[i];
        if (p > r) {
            expBinom[i] = 0;
            continue;
        }
        expBinom[i] = exp_fact(r, p) - exp_fact(k, p) - exp_fact(r - k, p);
    }

    /* Search largest divisor d < k. */
    for (idx_t d = k - 1; d >= 1; --d) {
        int tmp = (int)d;
        bool ok = true;

        /* Factor tmp using primes. */
        for (size_t i = 0; i < primeCnt && primes[i] * primes[i] <= tmp; ++i) {
            uint32_t p = primes[i];
            int cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            if (cnt > expBinom[i]) {
                ok = false;
                break;
            }
        }

        if (ok && tmp > 1) {   /* remaining prime factor > sqrt(d) */
            /* tmp is prime; find its exponent in binomial. */
            size_t idx = 0;
            while (idx < primeCnt && primes[idx] < (uint32_t)tmp)
                ++idx;
            if (idx == primeCnt || primes[idx] != (uint32_t)tmp)
                ok = false;
            else if (1 > expBinom[idx])
                ok = false;
        }

        if (ok) {
            free(expBinom);
            return (seq_t)d;
        }
    }

    free(expBinom);
    return (seq_t)1;  /* fallback: 1 always divides the binomial */
}
