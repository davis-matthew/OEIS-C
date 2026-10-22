/*
 * A390793  Number of ways to write n as x^4 + y^2 + 2*z^2 + (p-1)^2/2, where
 *          x,y,z are in the set {m >= 0: 2*m is practical}, and p is either 1
 *          or an odd prime.
 * https://oeis.org/A390793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y,z,p) | x,y,z ≥ 0, * 2*x,2*y,2*z practical, p = 1
 *            or odd prime, * x^4 + y^2 + 2*z^2 + (p-1)^2/2 = n }. * * The
 *            code enumerates all feasible (x,y,z) and counts the admissible
 *            p. * Practical numbers are tested by their prime factorisation
 *            and cached. * No external utilities are required
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Boolean array cache for "2*m is practical" ---------- */
static bool *practical_cache = NULL;   /* practical_cache[i] == true  ⇔  2*i is practical */
static idx_t practical_max = 0;        /* maximum m cached (inclusive) */

/* -------- Utility: integer square root (floor) for 64-bit values ------- */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low < high)
    {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* ----------- Test whether an unsigned 64‑bit number is practical ---- */
static inline bool is_practical_uint64(uint64_t n)
{
    if (n <= 1) return true;          /* 1 (and 0) are considered practical */
    if (n % 2 != 0) return false;     /* the prime factor 2 must appear */

    uint64_t m = n;
    int e2 = 0;
    while ((m & 1ULL) == 0)           /* remove the powers of two            */
    {
        m >>= 1;
        ++e2;
    }
    /* sum of divisors of 2^e2  =  2^(e2+1) – 1                      */
    unsigned __int128 sigma = ((unsigned __int128)1 << (e2 + 1)) - 1;

    /* factor the remaining odd part */
    for (uint64_t p = 3; p * p <= m; p += 2)
    {
        if (m % p != 0) continue;
        /* first prime beyond 2 must satisfy p ≤ σ + 1                 */
        if (p > sigma + 1) return false;

        int e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }

        /* divisor sum factor for p^e  = (p^(e+1) – 1)/(p – 1)          */
        unsigned __int128 pp = 1;
        for (int i = 0; i <= e; ++i) pp *= p;     /* pp = p^(e+1) */
        unsigned __int128 part = (pp - 1) / (p - 1);
        sigma *= part;
    }

    if (m > 1)          /* final prime factor (exponent 1) */
    {
        if (m > sigma + 1) return false;
        sigma *= (m + 1);   /* divisor sum of p^1  = p + 1                     */
    }

    return true;
}

/* ----------- Ensure cache covers all m ≤ needed --------------------- */
static void ensure_practical_up_to(idx_t needed)
{
    if (needed <= practical_max) return;
    size_t new_size = needed + 1;
    bool *new_arr = (bool *)realloc(practical_cache, new_size * sizeof(bool));
    if (!new_arr) abort();
    for (idx_t i = practical_max + 1; i <= needed; ++i)
        new_arr[i] = (i == 0) ? true : is_practical_uint64(2ULL * i);
    practical_cache = new_arr;
    practical_max   = needed;
}

/* ---------- Primitive primality test for 64‑bit unsigned ints ------ */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* --------- Helper: test if 2*m is practical, using the cache -------- */
static inline bool two_m_practical(uint64_t m)
{
    if (m == 0) return true;
    return practical_cache[m];
}

/* -------------------- Main sequence function ----------------------- */
seq_t a(idx_t n)
{
    /* For n = 0 the single decom­position (0,0,0,1) is counted. */
    if (n == 0) return (seq_t)1;

    /* Compute bounds for variables. */
    /* Bound for x: x^4 <= n */
    idx_t max_x = 0;
    while (1)
    {
        uint64_t x = max_x + 1;
        unsigned __int128 x4 = (unsigned __int128)x * x * x * x;
        if (x4 > n) break;
        ++max_x;
    }

    /* Bound for y: y^2 <= n */
    idx_t max_y = (idx_t)isqrt_u64(n);

    /* Bound for z: 2*z^2 <= n  →  z <= sqrt(n/2) */
    idx_t max_z = (idx_t)isqrt_u64(n / 2);

    /* Ensure the practicality cache is large enough. */
    idx_t cache_limit = max_x;
    if (max_y > cache_limit) cache_limit = max_y;
    if (max_z > cache_limit) cache_limit = max_z;
    ensure_practical_up_to(cache_limit);

    __uint128_t count = 0;

    /* Enumerate x */
    for (idx_t x = 0; x <= max_x; ++x)
    {
        if (x > 0 && !two_m_practical(x)) continue;
        unsigned __int128 x4 = (unsigned __int128)x * x * x * x;
        if (x4 > n) break;          /* safety, though loop bound already covers it */
        uint64_t rem_after_x = (uint64_t)(n - (uint64_t)x4);

        /* Enumerate y */
        uint64_t y_max = isqrt_u64(rem_after_x);
        for (uint64_t y = 0; y <= y_max; ++y)
        {
            if (y > 0 && !two_m_practical(y)) continue;
            uint64_t y2 = y * y;
            uint64_t rem_after_xy = rem_after_x - y2;

            /* Enumerate z */
            uint64_t z_max = isqrt_u64(rem_after_xy / 2);
            for (uint64_t z = 0; z <= z_max; ++z)
            {
                if (z > 0 && !two_m_practical(z)) continue;
                uint64_t z2 = z * z;
                uint64_t two_z2 = 2 * z2;
                if (two_z2 > rem_after_xy) break;
                uint64_t rem_after_xyz = rem_after_xy - two_z2;

                /* rem_after_xyz = (p-1)^2 / 2  →  2*rem = (p-1)^2 */
                uint64_t double_rem = rem_after_xyz * 2;
                uint64_t s = isqrt_u64(double_rem);
                if (s * s != double_rem) continue;   /* not a perfect square */

                uint64_t p = s + 1;
                if (p == 1 || (p & 1U && is_prime_uint64(p)))
                    ++count;
            }
        }
    }

    return (seq_t)count;
}
