/*
 * A361706  Inverse Moebius transform applied twice to primes.
 * https://oeis.org/A361706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A000005(n/d) * prime(d)
 * Range:     u8 n <= 23, u16 n <= 1259, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9521 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* where A000005 = τ(n) = number of divisors of n,
   and prime(k) = the k-th prime number. */

/* --------- divisor–count cache ------------------------------------- */
static idx_t max_tau_n = 0ull;
static idx_t *tau = NULL;

/* recompute τ(1)…τ(N) from scratch (N >= max_tau_n) */
static void ensure_tau(idx_t N)
{
    if (N <= max_tau_n) return;

    /* reallocate array to hold indices 0 … N */
    tau = (idx_t*)realloc(tau, (N + 1) * sizeof(idx_t));
    if (!tau) abort();

    /* reset all entries */
    for (idx_t i = 0; i <= N; ++i) tau[i] = 0ull;

    /* classic sieve‑like divisor counting */
    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = i; j <= N; j += i)
            ++tau[j];
    }
    max_tau_n = N;
}

/* ---------------- sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* sequence is defined for n ≥ 1 */

    ensure_tau(n);

    seq_t result = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;

            /* prime(d) and prime(d2) */
            seq_t pr1 = (seq_t)get_prime(d);
            seq_t pr2 = (seq_t)get_prime(d2);

            /* τ(n/d) * prime(d) */
            result += (seq_t)tau[n / d] * pr1;
            if (d2 != d)
                result += (seq_t)tau[n / d2] * pr2;
        }
    }
    return result;
}
