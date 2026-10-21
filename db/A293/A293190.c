/*
 * A293190  a(n) = |{A001597(n) <= k <= A001597(n+1): 2*k^2-1 is prime}|.
 * https://oeis.org/A293190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = |{A001597(n) <= k <= A001597(n+1) : 2*k^2-1 is prime}|
 * Range:     u8 n <= 997, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation builds a table of all perfect powers up to a fixed
    limit that guarantees more than 10 000 entries (the maximum n expected in
    the tests).  For each request it scans the integer interval between the
    two successive perfect powers and counts how many values of 2*k^2-1 are
    prime, using a deterministic Miller–Rabin test for 64‑bit numbers.
    The code complies with the C11/ C++17 interface required by the
    testing harness.                                                 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- helpers for comparing uint64_t -------------------------------- */
static int cmp_uint64(const void *p, const void *q)
{
    uint64_t a = *(const uint64_t *)p;
    uint64_t b = *(const uint64_t *)q;
    return (a < b) ? -1 : (a > b);
}

/* ---------- modular arithmetic for 64‑bit -------------------------------- */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t pow_mod_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1U)
            r = mul_mod_uint64(r, a, mod);
        a = mul_mod_uint64(a, a, mod);
        e >>= 1U;
    }
    return r;
}

/* ---------- Miller–Rabin primality test for 64‑bit ------------------------ */
static bool is_prime64(uint64_t n)
{
    if (n < 2U)
        return false;
    /* small primes */
    static const uint64_t sp[] = { 2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
                                   17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL };
    for (size_t i = 0; i < sizeof(sp) / sizeof(sp[0]); ++i)
    {
        if (n == sp[i])
            return true;
        if (n % sp[i] == 0U)
            return false;
    }

    /* write n‑1 = d·2^s with d odd */
    uint64_t d = n - 1U;
    unsigned s = 0U;
    while ((d & 1U) == 0U)
    {
        d >>= 1U;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0U)
            continue;
        uint64_t x = pow_mod_uint64(a, d, n);
        if (x == 1U || x == n - 1U)
            continue;
        bool composite = true;
        for (unsigned r = 1U; r < s; ++r)
        {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1U)
            {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ---------- build all perfect powers up to a fixed limit ----------------- */
static void build_perfect_powers(uint64_t limit, uint64_t **out, size_t *cnt)
{
    /* temporary dynamic array */
    size_t cap = 2048;
    uint64_t *tmp = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t sz = 0;

    tmp[sz++] = 1ULL;          /* 1 is a perfect power (1^b) */

    for (uint64_t exp = 2U; ; ++exp)
    {
        bool any = false;
        for (uint64_t base = 2U;; ++base)
        {
            __uint128_t val = 1U;
            for (uint64_t e = 0U; e < exp; ++e)
            {
                val *= base;
                if (val > limit)
                    break;
            }
            if (val > limit)
                break;
            uint64_t v = (uint64_t)val;
            if (sz == cap)
            {
                cap <<= 1;
                tmp = (uint64_t *)realloc(tmp, cap * sizeof(uint64_t));
            }
            tmp[sz++] = v;
            any = true;
        }
        if (!any)
            break;
    }

    /* sort and deduplicate */
    qsort(tmp, sz, sizeof(uint64_t), cmp_uint64);
    size_t uniq = 0;
    for (size_t i = 0; i < sz; ++i)
    {
        if (i == 0U || tmp[i] != tmp[i - 1U])
            tmp[uniq++] = tmp[i];
    }

    /* shrink to exact size */
    tmp = (uint64_t *)realloc(tmp, uniq * sizeof(uint64_t));
    *out = tmp;
    *cnt = uniq;
}

/* ---------- the sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    /* lazy static initialization of perfect powers */
    static bool built = false;
    static uint64_t *perfect = NULL;
    static size_t pcnt = 0U;

    if (!built)
    {
        /* limit chosen so that pcnt > 10 000 (our expected maximum n).   */
        const uint64_t LIMIT = 200000000ULL;      /* 2×10^8 */
        build_perfect_powers(LIMIT, &perfect, &pcnt);
        built = true;
    }

    if (n < 1U || (size_t)n + 1U >= pcnt)
        return 0U;          /* safety for out‑of‑range indices */

    uint64_t low = perfect[n - 1U];
    uint64_t high = perfect[n];

    seq_t count = 0U;
    for (uint64_t k = low; k <= high; ++k)
    {
        __uint128_t t = (__uint128_t)2U * k * k;
        uint64_t val = (uint64_t)(t - 1U);
        if (is_prime64(val))
            ++count;
    }

    return count;
}
