/*
 * A378540  a(n) = 1 if n is a practical number (A005153) that is neither a
 *          square or twice a square, otherwise a(n) = 0.
 * https://oeis.org/A378540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8700, u16 n <= 8700, u32 n <= 8700, u64 n <= 8700,
 *            u128 n <= 8700, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if n is a practical number and not a square or twice a square,
   otherwise a(n) = 0.  The test is performed by prime factorisation
   and the known practicality criterion. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Return true iff x is a perfect square.
   Uses binary search on 64‑bit integers. */
static bool is_square(idx_t x)
{
    if (x < 2) return true;          /* 0 and 1 are squares */
    idx_t low = 1, high = x >> 1;    /* sqrt(x) ≤ x/2 for x>1 */
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid <= x / mid) {
            if ((__int128)mid * mid == x) return true;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return false;
}

/* Compute base^exp in unsigned __int128. */
static unsigned __int128 pow_uint128(uint64_t base, idx_t exp)
{
    unsigned __int128 result = 1;
    unsigned __int128 b = base;
    while (exp) {
        if (exp & 1ULL) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

/* Test whether n (1 ≤ n < 2⁶⁴) is a practical number. */
static bool is_practical(idx_t n)
{
    if (n == 1) return true;          /* 1 is practical by definition */
    uint64_t primes[64];
    int      exps[64];
    size_t   cnt = 0;
    uint64_t m   = n;

    /* Factorise n by trial division. */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p) continue;
        int e = 0;
        while (!(m % p)) {            /* m % p == 0 */
            m /= p;
            ++e;
        }
        primes[cnt] = p;
        exps[cnt++] = e;
    }
    if (m > 1) {                      /* remaining prime factor */
        primes[cnt] = m;
        exps[cnt++] = 1;
    }
    if (cnt == 0) return false;       /* safeguard for n>1 */
    if (primes[0] != 2) return false; /* first prime must be 2 */

    unsigned __int128 sigma = 1;       /* σ of processed primes */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pi = primes[i];
        int      e  = exps[i];
        if (sigma < (unsigned __int128)(pi - 1)) return false;
        unsigned __int128 pi_pow = pow_uint128(pi, (idx_t)e + 1);
        unsigned __int128 sum   = (pi_pow - 1) / (pi - 1); /* σ(pi^e) */
        sigma *= sum;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* safety for bad calls */
    if (!is_practical(n)) return 0;
    if (is_square(n)) return 0;
    if ((n & 1ULL) == 0 && is_square(n / 2))   /* n = 2 * square */
        return 0;
    return (seq_t)1;
}
