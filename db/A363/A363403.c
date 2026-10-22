/*
 * A363403  a(n) = (4^n - 2^n) / Clausen(n, 1).
 * https://oeis.org/A363403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^n - 2^n) / Clausen(n, 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow2, pow2_modp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/pow2_modp.h"

/* Precompute all primes up to 10001 (maximum n + 1). */
static const idx_t MAX_N = 10000;
static idx_t prime_list[1229];      /* ≈ π(10001) */
static size_t prime_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    primes_initialized = true;

    bool is_prime[MAX_N + 2];
    for (idx_t i = 0; i <= MAX_N + 1; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= MAX_N + 1; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= MAX_N + 1; j += i)
                is_prime[j] = false;
        }
    }

    for (idx_t i = 2; i <= MAX_N + 1; ++i)
        if (is_prime[i]) prime_list[prime_cnt++] = i;
}

seq_t a(idx_t n)
{
    init_primes();

    /* Step 1: compute 2^n once. */
    seq_t two_n = pow2(n);                /* 2^n  */
    seq_t half   = two_n / 2;             /* 2^(n-1) for n>=1, 0 for n=0   */
    seq_t result = half * (two_n - 1);    /* numerator / 2  */

    /* Step 2: divide out each prime p with (p-1)|n and 2^n ≡ 1 (mod p). */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p == 2) continue;            /* 2 is already accounted for by the division above */
        if (p > n + 1) break;
        if ((n % (p - 1)) != 0) continue;
        if (pow2_modp(n, (uint64_t)p) == 1)
            result = result / (seq_t)p;
    }

    return result;
}
