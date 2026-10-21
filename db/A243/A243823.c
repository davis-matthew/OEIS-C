/*
 * A243823  Number of k < n such that gcd(k,n) > 1 but rad(k) does not divide
 *          n, where rad = A007947.
 * https://oeis.org/A243823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k < n | gcd(k,n) > 1 and rad(k) ∤ n }
 * Range:     u8 n <= 431, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a brute‑force loop over k; rad(k) is computed by
   trial division with the primes up to √k.  The primes are generated once
   up to 10000 (enough for all values needed by the specification).  */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"

static const idx_t PRIME_SIEVE_MAX = 10000;

static uint64_t primes[1229];   /* 1229 primes ≤ 10000 */
static size_t primes_cnt = 0;
static int primes_seeded = 0;

/* ---------- utilities ---------- */

static uint64_t rad_u64(uint64_t x)
{
    uint64_t rad = 1;
    uint64_t t = x;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > t) break;
        if (t % p == 0) {
            rad *= p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) rad *= t;          /* remaining prime factor */
    return rad;
}

static void seed_primes(void)
{
    if (primes_seeded) return;
    primes_seeded = 1;

    /* simple sieve */
    uint8_t mark[PRIME_SIEVE_MAX + 1];
    for (idx_t i = 0; i <= PRIME_SIEVE_MAX; ++i) mark[i] = 1;
    mark[0] = mark[1] = 0;
    for (idx_t i = 2; i * i <= PRIME_SIEVE_MAX; ++i) {
        if (mark[i]) {
            for (idx_t j = i * i; j <= PRIME_SIEVE_MAX; j += i)
                mark[j] = 0;
        }
    }
    for (idx_t i = 2; i <= PRIME_SIEVE_MAX; ++i)
        if (mark[i]) primes[primes_cnt++] = i;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* offset 1 */

    seed_primes();                 /* ensure primes are ready */

    uint64_t cnt = 0;
    for (idx_t k = 2; k < n; ++k) {
        if (gcd_u64(k, n) > 1) {   /* gcd(k,n) > 1 */
            uint64_t r = rad_u64(k);
            if (n % r != 0) ++cnt;
        }
    }
    return (seq_t)cnt;             /* cast back to external type */
}
