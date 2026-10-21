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
 * Helpers:   get_prime, pow_mod_u128
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
#include "../utils/get_prime.h"  /* provides uint32_t get_prime(idx_t) */
#include "../utils/pow_mod_u128.h"

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
