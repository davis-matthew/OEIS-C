/*
 * A361231  a(1)=2; a(n) is the largest k for which the sum a(n-1) + a(n-2) +
 *          ... + a(n-k) is prime; if no such k exists, a(n)=-1.
 * https://oeis.org/A361231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition of OEIS sequence A361231
   a(1)=2; a(2)=1;
   For n>2, a(n)=largest k such that a(n-1)+a(n-2)+...+a(n-k) is prime.
   If no such k exists, a(n) = -1.  Recompute terms in any order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Sequence types defined by the build system */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Internal prime test for 64‑bit unsigned integers (deterministic) */

static uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((unsigned __int128)a * b % mod);
}

static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1) res = mul_mod_uint64(res, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

static bool is_prime_int64(int64_t n)
{
    if (n < 2) return false;
    uint64_t x = (uint64_t)n;
    if ((x & 1ULL) == 0ULL) return x == 2ULL;

    /* trial division by small primes first */
    static const int small_primes[] =
        {3,5,7,11,13,17,19,23,29,31,37,41,43,47};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        int p = small_primes[i];
        if (x % p == 0ULL) return x == (uint64_t)p;
    }

    /* Miller–Rabin with known bases for 64‑bit integers */
    uint64_t d = x - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % x;
        if (a == 0ULL) return true;
        uint64_t x_a = pow_mod_uint64(a, d, x);
        if (x_a == 1ULL || x_a == x - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x_a = mul_mod_uint64(x_a, x_a, x);
            if (x_a == x - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Memoized computation of the sequence */

static int64_t *terms_s = NULL;   /* signed values for internal use */
static int64_t *pref_s  = NULL;   /* signed prefix sums */
static idx_t   cap     = 0;       /* largest index computed */

static void ensure_cache(idx_t n)
{
    if (n <= cap) return;

    size_t newsize = (size_t)(n + 1);   /* include index 0 */
    terms_s = (int64_t*)realloc(terms_s, newsize * sizeof(int64_t));
    pref_s  = (int64_t*)realloc(pref_s,  newsize * sizeof(int64_t));

    if (cap == 0) pref_s[0] = 0;        /* first allocation */

    for (idx_t i = cap + 1; i <= n; ++i) {
        int64_t val;
        if (i == 1) {
            val = 2;
        } else if (i == 2) {
            val = 1;
        } else {
            int best = -1;
            for (idx_t k = i - 1; k >= 1; --k) {
                int64_t sum = pref_s[i - 1] - pref_s[i - k - 1];
                if (sum >= 2 && is_prime_int64(sum)) {
                    best = (int)k;
                    break; /* largest k found */
                }
                if (k == 1) break;
            }
            val = best;
        }
        terms_s[i] = val;
        pref_s[i] = pref_s[i - 1] + val;
    }
    cap = n;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return (seq_t)terms_s[n];
}
