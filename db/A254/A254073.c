/*
 * A254073  Number of solutions to x^3 + y^3 + z^3 == 1 (mod n) for 1 <= x, y,
 *          z <= n.
 * https://oeis.org/A254073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} a(p^e), where a(p^e) = (p^e)^2 if 3∤φ(p^e)
 *            (i.e. p≡2 mod 3 or p=3,e=1), otherwise a(p^e) is computed by
 *            brute‑force enumeration of residues modulo p^e
 * Range:     u8 n <= 13, u16 n <= 170, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pp_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pp_square.h"

/* Maximum n considered */
#define MAXN 10000

/* Cache for full terms a(n) (n ≤ MAXN) */
static seq_t cache_n[MAXN + 1] = {0};

/* Prime list – generated once. */
static uint32_t primes[2000];
static size_t prime_count = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;
    static bool is_prime[MAXN + 1];
    for (size_t i = 2; i <= MAXN; ++i) is_prime[i] = true;
    for (size_t i = 2; i * i <= MAXN; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= MAXN; j += i)
                is_prime[j] = false;
    for (size_t i = 2; i <= MAXN; ++i)
        if (is_prime[i]) primes[prime_count++] = (uint32_t)i;
}

/* Cache for special prime‑power counts a(p^e) (p^e ≤ MAXN). */
static seq_t cache_pp[MAXN + 1] = {0};

/* Enumerative computation for non‑trivial prime powers. */
static seq_t pp_special(idx_t p, idx_t e)
{
    idx_t mod = 1;
    for (idx_t k = 0; k < e; ++k) mod *= p;

    if (cache_pp[mod] != 0) return cache_pp[mod];

    size_t m = (size_t)mod;
    static uint32_t cnt[MAXN + 1];
    for (size_t i = 0; i < m; ++i) cnt[i] = 0;

    /* Count x³ mod m for all x. */
    for (size_t x = 0; x < m; ++x) {
        size_t r = ((size_t)x * x % m) * x % m;
        ++cnt[r];
    }

    /* Triple sum. */
    uint64_t sum = 0;
    for (size_t i = 0; i < m; ++i) {
        if (cnt[i] == 0) continue;
        for (size_t j = 0; j < m; ++j) {
            if (cnt[j] == 0) continue;
            int diff = 1 - (int)i - (int)j;
            int k = diff % (int)m;
            if (k < 0) k += m;
            if (cnt[(size_t)k] == 0) continue;
            sum += (uint64_t)cnt[i] * cnt[j] * cnt[(size_t)k];
        }
    }

    seq_t result = (seq_t)sum;
    cache_pp[mod] = result;
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* Outside the OEIS offset, undefined. */
    if (cache_n[n] != 0) return cache_n[n];

    if (n == 1) {
        cache_n[1] = 1;
        return 1;
    }

    init_primes();

    idx_t tmp = n;
    seq_t result = 1;

    for (size_t i = 0; i < prime_count && (idx_t)primes[i] * primes[i] <= tmp; ++i) {
        idx_t p = primes[i];
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t part;
            if ((p == 3 && e == 1) || (p % 3 == 2))
                part = pp_square(p, e);
            else
                part = pp_special(p, e);
            result *= part;
        }
    }
    if (tmp > 1) {  /* Remaining prime factor. */
        idx_t p = tmp;
        seq_t part;
        if ((p == 3 && 1 == 1) || (p % 3 == 2))
            part = pp_square(p, 1);
        else
            part = pp_special(p, 1);
        result *= part;
    }

    cache_n[n] = result;
    return result;
}
