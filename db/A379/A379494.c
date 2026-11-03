/*
 * A379494  a(n) = 2*A276086(n) - A276086(A001065(n)), where A276086 is the
 *          primorial base exp-function, and A001065 is the sum of proper
 *          divisors of n.
 * https://oeis.org/A379494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*A276086(n) - A276086(A001065(n))
 * Range:     i8 n <= 10, i16 n <= 56, i32 n <= 208, i64 n <= 2059,
 *            i128 n <= 30030, big any n
 * Validated: standard, pass, 74 terms through n = 8316 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The primorial base exp-function A276086 is implemented below: */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t primorial_exp(seq_t m)
{
    /* Compute A276086(m): product of prime^digit where digits are the mixed-radix
       representation of m with bases equal to successive primes. */
    if (m == 0) return 1;          /* A276086(0) is defined as 1 */
    seq_t result = 1;
    seq_t weight = 1;              /* product of primes used as weights */
    idx_t k = 1;                   /* 1‑based index of the current prime */

    while (weight <= m)
    {
        uint64_t prime = get_prime(k);           /* current prime p_k */
        seq_t digit = (m / weight) % prime;      /* a_k  (0 <= a_k < prime) */

        /* multiply result by prime^digit */
        seq_t mult = 1;
        for (seq_t i = 0; i < digit; ++i)
            mult *= prime;
        result *= mult;

        /* advance weight: weight *= p_k for the next round */
        weight *= prime;
        ++k;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* A276086(n) */
    seq_t f_n = primorial_exp((seq_t)n);

    /* A001065(n) = sum of proper divisors = sigma_of(n) - n */
    seq_t proper_sum = sigma_of(n) - (seq_t)n;

    /* A276086(A001065(n)) */
    seq_t f_sum = primorial_exp(proper_sum);

    /* 2*A276086(n) - A276086(A001065(n)) */
    return f_n + f_n - f_sum;
}
