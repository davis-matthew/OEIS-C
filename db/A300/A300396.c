/*
 * A300396  Number of ways to write n^2 as x^2 + y^2 + z^2 + w^2 with x,y,z,w
 *          nonnegative integers and z <= w such that 2*x or y is a power of 4
 *          (including 4^0 = 1) and x + 63*y = 2^(2k+1) for some k =
 *          0,1,2,....
 * https://oeis.org/A300396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of ways to write n^2 as x^2 + y^2 + z^2 + w^2 with
 *            x,y,z,w ≥ 0, z ≤ w, and (2*x) or y a power of 4, while x + 63*y
 *            = 2^(odd exponent)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This code enumerates all admissible pairs (x,y), then counts
    unordered representations of the remaining sum as two squares
    using the classical divisor formula. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

/* --- power‑of‑four test ------------------------------------- */
static inline bool is_power_of_four(uint64_t x)
{
    if (x == 0) return false;
    return (x & (x-1)) == 0 && (__builtin_ctzll(x) & 1) == 0;
}

/* --- integer sqrt + square test ------------------------------- */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t r = (uint64_t)sqrt((long double)n);
    while ((r+1)*(r+1) <= n) ++r;
    while (r*r > n) --r;
    return r;
}

static inline bool is_square_u64(uint64_t n)
{
    uint64_t r = isqrt_u64(n);
    return r*r == n;
}

/* --- small prime sieve up to 10000 --------------------------- */
#define PRIME_MAX 10000
static uint32_t primes[PRIME_MAX];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt) return;
    bool is_prime[PRIME_MAX+1];
    for (int i = 2; i <= PRIME_MAX; ++i) is_prime[i] = true;
    for (int i = 2; i * i <= PRIME_MAX; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= PRIME_MAX; j += i) is_prime[j] = false;
    for (int i = 2; i <= PRIME_MAX; ++i)
        if (is_prime[i]) primes[prime_cnt++] = (uint32_t)i;
}

/* --- count unordered (z,w) with z^2 + w^2 = s ----------------- */
static seq_t count_two_squares_unordered(uint64_t s)
{
    if (s == 0) return (seq_t)1;

    uint64_t rem = s;
    uint64_t mul = 1;                 /* product over primes ≡1 (mod 4) */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > rem) break;
        if (rem % p == 0) {
            int exp = 0;
            while (rem % p == 0) { rem /= p; ++exp; }
            if (p == 2) {
                /* ignore p=2 completely */
                continue;
            }
            if (p % 4 == 3) {
                if (exp & 1) return (seq_t)0;        /* odd exponent of 3‑mod‑4 prime */
            } else { /* p ≡ 1 (mod 4) */
                mul *= (uint64_t)(exp + 1);
            }
        }
    }

    if (rem > 1) {             /* rem is prime > sqrt(rem) */
        if (rem % 4 == 3) return (seq_t)0;
        mul *= 2;               /* prime ≡ 1 (mod 4) contributes factor (0+1)+1 = 2 */
    }

    uint64_t r2 = 4 * mul;      /* ordered representations with signs */
    bool B = is_square_u64(s);                  /* z = 0, w > 0 */
    bool C = ((s & 1ULL) == 0) && is_square_u64(s >> 1); /* z = w > 0, even s */
    uint64_t R = (r2 - (B?4ULL:0ULL) - (C?4ULL:0ULL)) / 8ULL
                 + (B?1ULL:0ULL) + (C?1ULL:0ULL);
    return (seq_t)R;
}

/* --- main OEIS sequence function ----------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) { init_primes(); initialized = true; }

    uint64_t N   = (uint64_t)n;
    uint64_t n2  = N * N;
    seq_t total  = 0;

    for (uint64_t y = 0; y <= N; ++y) {
        bool y_pow4 = is_power_of_four(y);
        uint64_t threshold = 63UL * y;          /* 63*y  */
        uint64_t max_T = N + threshold;        /* largest allowed T = x+63y */

        /* Find first power of two with odd exponent ≥ threshold. */
        uint64_t T = 2;                          /* 2^1 */
        while (T < threshold) T <<= 2;           /* multiply by 4 each step */

        for ( ; T <= max_T; T <<= 2) {
            uint64_t x = T - threshold;          /* x + 63*y = T */
            if (x > N) break;                    /* larger T → larger x */

            if (!y_pow4) {                       /* y not a power of 4 */
                /* require 2*x to be a power of 4 → x must be 2^(odd exponent) */
                if (!((x != 0) && ((x & (x-1)) == 0) && ((__builtin_ctzll(x) & 1) == 1)))
                    continue;
            }

            uint64_t xx = x * x;                  /* x^2 */
            uint64_t yy = y * y;                  /* y^2 */
            if (xx + yy > n2) continue;           /* too large */

            uint64_t S = n2 - xx - yy;
            total += count_two_squares_unordered(S);
        }
    }
    return total;
}
