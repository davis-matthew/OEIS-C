/*
 * A379238  a(n) = 1 if A003961(n)-sigma(n) is prime, otherwise 0, where
 *          A003961 is fully multiplicative with a(prime(i)) = prime(i+1), and
 *          sigma is the sum of divisors function.
 * https://oeis.org/A379238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A003961(n) - sigma(n) is prime, else 0 * where
 *            A003961 is fully multiplicative with a(prime(i)) = prime(i+1) *
 *            and sigma is the sum of divisors function
 * Range:     u8 n <= 47, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 136 terms through n = 9412 (2026-11-03)
 * Helpers:   ipow_idx, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

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

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n <= 2)
        return 0;

    /* Factorize n using trial division (sufficient for n ≤ 10000) */
    uint64_t temp = (uint64_t)n;
    seq_t prod = 1;

    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            unsigned exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            uint64_t next_p = next_prime(p);
            seq_t val = ipow_idx((seq_t)next_p, exp);
            prod *= val;
        }
    }
    if (temp > 1) /* remaining prime factor */
    {
        uint64_t next_p = next_prime(temp);
        prod *= ipow_idx((seq_t)next_p, 1);
    }

    seq_t sigma = sigma_of(n);
    seq_t diff = prod - sigma;

    if (diff <= (seq_t)1)
        return 0;

    return is_prime_big(diff) ? 1 : 0;
}
