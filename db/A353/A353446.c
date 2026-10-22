/*
 * A353446  Let g be the inverse Möbius transform of the Eisenstein
 *          integer-valued function f defined in A353445. a(n) is twice the
 *          real part of g(n).
 * https://oeis.org/A353446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*Re(g(n)) where g(n) is the multiplicative * function
 *            defined by * g(p^e) = 1 if e % 3 == 0 * g(p^e) = 0 if e % 3 == 2
 *            * g(p^e) = 1+w (i.e. ζ) if e % 3 == 1 and the prime p has odd
 *            index, * g(p^e) = -w (i.e. ζ⁵) if e % 3 == 1 and the prime p has
 *            even index, * with w = exp(2πi/3). The prod...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Prime generation up to a fixed limit (sufficient for all tests) */
static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;
static void init_primes(void)
{
    if (prime_list != NULL)
        return;
    const size_t LIMIT = 1000000;   /* 1e6 is enough for all test values */
    bool *is_composite = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= LIMIT; ++i)
        is_composite[i] = false;
    is_composite[0] = is_composite[1] = true;
    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_composite[j] = true;
    /* count primes */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i])
            ++prime_cnt;
    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i])
            prime_list[idx++] = (uint32_t)i;
    free(is_composite);
}

static size_t prime_index(uint32_t p)
{
    /* binary search; p is known to be prime and ≤ max prime in list */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo; /* 0‑based index */
}

seq_t a(idx_t n)
{
    init_primes();

    uint64_t nn = n;
    int exp_sum = 0;              /* value modulo 6, 0..5 */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > nn)
            break;
        if (nn % p == 0) {
            uint64_t e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            uint64_t r = e % 3;
            if (r == 2)          /* factor contributes 0 -> total 0 */
                return (seq_t)0;
            if (r == 1) {
                size_t idx = prime_index(p);
                bool odd_index = ((idx + 1) & 1) == 1;   /* 1‑based */
                if (odd_index)
                    exp_sum = (exp_sum + 1) % 6;
                else
                    exp_sum = (exp_sum + 5) % 6;
            }
        }
    }

    if (nn > 1) {                       /* remaining prime factor */
        uint32_t p = (uint32_t)nn;       /* nn fits in 32 bits here */
        /* e == 1 -> r == 1 */
        size_t idx = prime_index(p);
        bool odd_index = ((idx + 1) & 1) == 1;
        if (odd_index)
            exp_sum = (exp_sum + 1) % 6;
        else
            exp_sum = (exp_sum + 5) % 6;
    }

    switch (exp_sum) {                  /* mapping 0..5 → a(n)  */
        case 0: return (seq_t)2;
        case 1: return (seq_t)1;
        case 2: return (seq_t)-1;
        case 3: return (seq_t)-2;
        case 4: return (seq_t)-1;
        case 5: return (seq_t)1;
        default: return (seq_t)0;       /* should never happen */
    }
}
