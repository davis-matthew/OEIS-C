/*
 * A349203  Triangle read by rows, T(n, k) = (lcm_{k=0..n} binomial(n, k)) /
 *          binomial(n, k).
 * https://oeis.org/A349203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangle T(n,k) read row‑wise, where * T(m,k) = (
 *            lcm_{j=0..m} binomial(m,j) ) / binomial(m,k)
 * Range:     u8 n <= 35, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gt_half_prod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) enumerates the entries T(m,k) in row‑major order. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gt_half_prod.h"

/* ---------- prime table ---------- */
#define PRIMES_LIMIT 200000          /* covers all needed rows for n≈10⁴   */
static unsigned char prime_flag[PRIMES_LIMIT+1];
static uint32_t primes[PRIMES_LIMIT/10 + 10];
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_count) return;
    for (size_t i = 2; i <= PRIMES_LIMIT; ++i) {
        if (!prime_flag[i]) {
            primes[prime_count++] = (uint32_t)i;
            if (i * i <= PRIMES_LIMIT) {
                for (size_t j = i * i; j <= PRIMES_LIMIT; j += i)
                    prime_flag[j] = 1;
            }
        }
    }
}

/* ---------- factorial v_p helper ---------- */
static idx_t vp_factorial(idx_t n, uint32_t p)
{
    idx_t cnt = 0;
    for (idx_t d = p; d <= n; d *= p) {
        cnt += n / d;
        if (d > n / p) break;     /* avoid overflow of d *= p   */
    }
    return cnt;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    init_primes();

    /* find row m and column k such that a(n) = T(m,k) */
    idx_t low = 0, high = 1;
    while (!gt_half_prod(high + 1, high + 2, n))
        high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (gt_half_prod(mid + 1, mid + 2, n))
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;
    idx_t prev = m * (m + 1) / 2;          /* elements before row m   */
    idx_t k = n - prev;                    /* column in row m         */

    /* compute T(m,k) = L(m) / C(m,k)   ,   L(m) = lcm(1..m+1)/(m+1) */
    seq_t result = (seq_t)1;
    idx_t limit = m + 1;

    for (size_t pi = 0; pi < prime_count && primes[pi] <= limit; ++pi) {
        uint32_t p = primes[pi];

        /* exponent of p in the lcm(1..m+1)  */
        idx_t e_lcm = 0;
        for (idx_t pp = p; pp <= limit; pp *= p)
            ++e_lcm;

        /* subtract exponent of p in (m+1)  */
        idx_t temp = limit;
        idx_t exp_m1 = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp_m1;
        }
        idx_t e_target = (e_lcm > exp_m1) ? e_lcm - exp_m1 : 0;
        if (e_target == 0) continue;       /* no contribution from this prime */

        /* exponent of p in C(m,k) */
        idx_t vp_m  = vp_factorial(m,       p);
        idx_t vp_k  = vp_factorial(k,       p);
        idx_t vp_mk = vp_factorial(m - k,   p);
        idx_t vp_C  = vp_m - vp_k - vp_mk;

        idx_t expo = (e_target > vp_C) ? e_target - vp_C : 0;
        if (expo == 0) continue;

        /* multiply result by p^expo */
        seq_t powv = (seq_t)1;
        for (idx_t i = 0; i < expo; ++i)
            powv *= (seq_t)p;
        result *= powv;
    }

    return result;
}
