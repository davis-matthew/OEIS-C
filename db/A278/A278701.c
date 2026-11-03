/*
 * A278701  Square array A(n, k) read by antidiagonals downwards: A(n,k) =
 *          characteristic function of base-n Wieferich primes: 1 if prime(k)
 *          is a base-n Wieferich prime, 0 otherwise, where k runs over the
 *          positive integers starting from 1.
 * https://oeis.org/A278701
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the characteristic function of base‑n Wieferich primes
 *            read by antidiagonals downwards starting with (n,k) = (2,1)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 88 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility: integer square root of an unsigned __int128 value. */
static unsigned long long isqrt_u128(unsigned __int128 x)
{
    unsigned long long lo = 0, hi = ((unsigned long long)1 << 32);
    while (lo < hi) {
        unsigned long long mid = (lo + hi) >> 1;
        if ((unsigned __int128)mid * mid > x)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo - 1;
}

/* Main sequence function */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Modulo exponentiation using __int128 to avoid overflow in small seq_t builds. */
static unsigned __int128 pow_mod_u128(unsigned __int128 base,
                                      uint64_t exp,
                                      unsigned __int128 mod)
{
    unsigned __int128 res = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* index below offset */
    /* index into pairs: target = number of pairs before the current pair */
    idx_t target = n - 2;

    /* Solve for diagonal s: F(s) = (s-2)(s-1)/2 > target */
    unsigned __int128 d = (unsigned __int128)1 + (unsigned __int128)8 * target;
    unsigned long long sqrt_d = isqrt_u128(d);
    unsigned long long s = (sqrt_d + 3) >> 1;   /* (sqrt_d + 3)/2 */

    /* Ensure we have the minimal s with F(s) > target */
    unsigned __int128 F_s = ((unsigned __int128)(s - 2) * (s - 1)) >> 1;
    while (F_s <= target) {
        ++s;
        F_s = ((unsigned __int128)(s - 2) * (s - 1)) >> 1;
    }

    /* Number of pairs before this diagonal */
    unsigned __int128 G_s = ((unsigned __int128)(s - 3) * (s - 2)) >> 1;
    idx_t pos = target - (idx_t)G_s;          /* 0‑based position in diagonal */

    idx_t n_base = 2 + pos;      /* base n >= 2 */
    idx_t k_prime = s - n_base;  /* k >= 1 */

    /* Get the k‑th prime number (1‑based).  get_prime returns uint32_t. */
    uint32_t prime_k = get_prime(k_prime);

    /* Compute n_base^(prime_k-1) mod prime_k^2 */
    unsigned __int128 mod = (unsigned __int128)prime_k * prime_k;
    unsigned __int128 exp = (unsigned __int128)prime_k - 1;
    unsigned __int128 val = pow_mod_u128((unsigned __int128)n_base, (uint64_t)exp, mod);

    return (val == 1) ? 1 : 0;
}
