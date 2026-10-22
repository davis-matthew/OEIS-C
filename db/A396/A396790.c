/*
 * A396790  Triangle read by rows: T(n, k) = least hypotenuse z of a primitive
 *          Pythagorean triple whose legs are multiples p*x and q*y, where p =
 *          prime(n), q = prime(k), 1 <= k < n.
 * https://oeis.org/A396790
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = least hypotenuse z of a primitive Pythagorean triple
 *            whose legs are multiples of prime(n) and prime(k), read row-wise
 *            from the triangle T(n,k). See OEIS A396790
 * Range:     u8 n <= 28, u16 n <= 1540, u32 n <= 9719, u64 n <= 9719,
 *            u128 n <= 9719, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- prime sieve (static, one‑time) ---------- */
#define PRIME_LIMIT 200000       /* > 10 000‑th prime = 104 729 */
static uint64_t primes[20000];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void make_primes(void)
{
    if (primes_ready) return;
    static unsigned char sieve[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) primes[prime_cnt++] = i;
    primes_ready = true;
}

static inline uint64_t get_prime(idx_t idx)   /* 1‑based */
{
    make_primes();
    return primes[idx - 1];
}

/* ---------- helper function: gcd of seq_t values ---------- */
#include "../utils/gcd.h"

/* ---------- solve ------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* value is undefined below offset */

    /* convert linear index to (row, column) in the triangle */
    idx_t m = n - 2;                        /* zero‑based index in the flattened triangle */

    /* find smallest r such that r*(r-1)/2 > m   (row r has r-1 elements) */
    idx_t low = 2, high = 2;
    while ((high * (high - 1)) / 2 <= m) high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((mid * (mid - 1)) / 2 > m) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;                          /* row index, 1‑based (prime index) */

    idx_t prev = (r - 2) * (r - 1) / 2;      /* number of terms before row r */
    idx_t c = m - prev + 1;                 /* column index, 1‑based (prime index) */

    uint64_t p = get_prime(r);              /* odd prime p = prime(r) */
    uint64_t q = get_prime(c);              /* odd prime q = prime(c) */

    /* search for minimal primitive hypotenuse z = u^2 + v^2 */
    unsigned __int128 best_z = (unsigned __int128)-1;   /* initial sentinel */

    /* u runs over positive integers.  For each u we try the two possible v
       that satisfy p | (u^2 - v^2)  (i.e., u ≡ ±v (mod p)).           */
    for (uint64_t u = 1; ; ++u) {
        /* safety stop: if u^2 alone exceeds current best, we cannot improve */
        if (best_z != (unsigned __int128)-1 && (unsigned __int128)u * u >= best_z)
            break;

        uint64_t modp = u % p;

        /* Candidate 1: v ≡  u (mod p)  (v = modp)  */
        if (modp != 0) {
            uint64_t v = modp;
            if (v < u && ((u - v) & 1) && gcd((seq_t)u, (seq_t)v) == 1) {
                bool ok = (q == 2) || (u % q == 0) || (v % q == 0);
                if (ok) {
                    unsigned __int128 z = (unsigned __int128)u * u
                                       + (unsigned __int128)v * v;
                    if (z < best_z) best_z = z;
                }
            }
        }

        /* Candidate 2: v ≡ -u (mod p)  (v = p - modp, unless modp==0) */
        uint64_t v2 = (modp == 0) ? p : (p - modp);
        if (v2 > 0 && v2 < u && ((u - v2) & 1) && gcd((seq_t)u, (seq_t)v2) == 1) {
            bool ok = (q == 2) || (u % q == 0) || (v2 % q == 0);
            if (ok) {
                unsigned __int128 z = (unsigned __int128)u * u
                                   + (unsigned __int128)v2 * v2;
                if (z < best_z) best_z = z;
            }
        }
    }

    return (seq_t)best_z;
}
