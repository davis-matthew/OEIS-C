/*
 * A337910  Integers of the form (the number of nonnegative bases m < n such
 *          that m^3 == m (mod n))/(the number of nonnegative bases m < n such
 *          that -m^3 == m (mod n)).
 * https://oeis.org/A337910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = (# of m < n with m³ ≡ m (mod n)) / (# of m < n with m³ ≡
 *            –m (mod n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* maximum n handled (tests use n ≤ 10000) */
#define MAX_N 10000

/* pre‑computed counts for each prime power p^e ≤ MAX_N */
static uint16_t cnt1[MAX_N + 1];   /* solutions of x³ ≡ x (mod p^e)   */
static uint16_t cnt2[MAX_N + 1];   /* solutions of x³ ≡ –x (mod p^e)  */
static uint16_t spf[MAX_N + 1];    /* smallest prime factor          */
static bool init_done = false;

/* ----- helper: sieve for smallest prime factors ----- */
static void
init_spf(void)
{
    for (idx_t i = 0; i <= MAX_N; ++i) spf[i] = i;
    for (idx_t i = 2; i * i <= MAX_N; ++i) if (spf[i] == i) {
        for (idx_t j = i * i; j <= MAX_N; j += i)
            if (spf[j] == j) spf[j] = i;
    }
}

/* ----- helper: pre‑compute counts for every prime power ----- */
static void
init_counts(void)
{
    for (idx_t p = 2; p <= MAX_N; ++p) if (spf[p] == p) {     /* p is prime */
        uint64_t power = p;
        while (power <= MAX_N) {
            uint64_t mod = power;
            uint16_t c1 = 0, c2 = 0;
            for (uint64_t x = 0; x < mod; ++x) {
                uint64_t x2 = (x * x) % mod;
                if ((x2 * x) % mod == x % mod) ++c1;           /* x³ ≡ x */
                if ((x2 * x) % mod == (mod - x) % mod) ++c2;   /* x³ ≡ –x */
            }
            cnt1[power] = c1;
            cnt2[power] = c2;
            power *= p;
        }
    }
}

/* ----- single initialization routine ----- */
static void
ensure_init(void)
{
    if (!init_done) {
        init_spf();
        init_counts();
        init_done = true;
    }
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside OEIS interval; safety */

    /* handle n = 1 explicitly */
    if (n == 1) return 1;

    ensure_init();

    seq_t prod_num = 1;   /* numerator: solutions to x³ ≡ x */
    seq_t prod_den = 1;   /* denominator: solutions to x³ ≡ –x */

    uint64_t tmp = (uint64_t)n;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p != 0) continue;
        uint64_t power = 1;
        while (tmp % p == 0) {
            tmp /= p;
            power *= p;
        }
        prod_num *= cnt1[power];
        prod_den *= cnt2[power];
    }

    if (tmp > 1) { /* remaining prime factor */
        uint64_t power = tmp;
        prod_num *= cnt1[power];
        prod_den *= cnt2[power];
    }

    return prod_num / prod_den;
}
