/*
 * A319855  Minimum number that can be obtained by iteratively adding or
 *          multiplying together parts of the integer partition with Heinz
 *          number n until only one part remains.
 * https://oeis.org/A319855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n = 1 * 1 if n is a power of 2 * Σ [e_p *
 *            prime_index(p)] otherwise * * Here the sum runs over all odd
 *            prime divisors p of n, e_p is the * exponent of p in the prime
 *            factorisation of n, and prime_index(p) * is the 1‑based index of
 *            p in the sequence of primes (2,3,5,7,…). * This is equ...
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/sieve.h"

static uint8_t *prime_mark = NULL;
static size_t  p_mark_size = 0;
static uint64_t *prime_list = NULL;
static size_t    prime_cnt  = 0;
static int      *prime_index = NULL;   /* 1‑based prime indices */

/* Ensure that all primes up to n are available.  The tables are
 * rebuilt when a larger n is requested. */
static void
ensure_primes(idx_t n)
{
    if (n < (idx_t)p_mark_size)
        return;          /* already prepared */

    /* Allocate a new boolean array for the sieve. */
    uint8_t *new_mark = (uint8_t*)malloc((size_t)(n + 1));
    if (!new_mark) exit(1);   /* allocation failure */
    for (size_t i = 0; i <= n; ++i) new_mark[i] = 1;
    new_mark[0] = new_mark[1] = 0;

    size_t lim = (size_t)sqrt((double)n);
    for (size_t i = 2; i <= lim; ++i) {
        if (new_mark[i]) {
            for (size_t j = i * i; j <= n; j += i)
                new_mark[j] = 0;
        }
    }

    /* Count primes and allocate arrays for them. */
    size_t cnt = 0;
    for (size_t i = 2; i <= n; ++i)
        if (new_mark[i]) ++cnt;

    uint64_t *new_primes = NULL;
    if (cnt > 0)
        new_primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));

    int *new_index = (int*)malloc((n + 1) * sizeof(int));
    if (!new_mark || !new_index) exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= n; ++i) {
        if (new_mark[i]) {
            new_primes[idx]   = (uint64_t)i;
            new_index[i] = (int)(idx + 1);  /* 1‑based index */
            ++idx;
        }
    }

    /* Replace old buffers. */
    free(prime_mark);
    free(prime_list);
    free(prime_index);

    prime_mark   = new_mark;
    prime_list   = new_primes;
    prime_cnt    = cnt;
    prime_index  = new_index;
    p_mark_size  = (size_t)n + 1;
}

seq_t
a(idx_t n)
{
    if (n == 1) return 0;

    /* Make sure the prime tables cover n. */
    ensure_primes(n);

    seq_t weight = 0;
    uint64_t temp = (uint64_t)n;

    /* Trial division by stored primes. */
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (p != 2) {
                weight += (seq_t)exp * (seq_t)prime_index[p];
            }
        }
    }

    /* If a prime factor greater than sqrt(n) remains. */
    if (temp > 1) {
        uint64_t p = temp;
        if (p != 2) {
            weight += (seq_t)prime_index[p];  /* exponent is 1 */
        }
    }

    /* For a pure power of 2 return 1. */
    if (weight == 0) weight = 1;
    return weight;
}
