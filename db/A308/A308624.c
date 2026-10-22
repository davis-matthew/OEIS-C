/*
 * A308624  Triangle read by rows: T(n,k) = n!/((n mod k)!*(k-(n mod
 *          k))!*(ceiling(n/k)!^(n mod k))*(floor(n/k)!^(k-(n mod k)))), n >=
 *          1, n >= k >= 1, the number of ways of dividing n labeled items
 *          into k unlabeled boxes as evenly as possible.
 * https://oeis.org/A308624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r = the unique integer with r(r+1)/2 ≥ n
 *            (row index), k = n - (r-1)r/2 (column index, 1‑based), and
 *            T(r,k) = r! / ((r mod k)! * (k-(r mod k))! * ceil(r/k)!^(r mod
 *            k) * floor(r/k)!^(k-(r mod k)))
 * Range:     u8 n <= 30, u16 n <= 70, u32 n <= 176, u64 n <= 442,
 *            u128 n <= 1140, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mul_small.h"   // seq_t mul_small(seq_t x, unsigned int m);

/* Return the exponent of prime p in m! */
static unsigned long long exp_factorial(idx_t m, int p)
{
    unsigned long long e = 0;
    idx_t div = (idx_t)p;
    while (div <= m) {
        e += m / div;
        if (div > m / (idx_t)p) break;
        div *= (idx_t)p;
    }
    return e;
}

seq_t a(idx_t n)
{
    /* Prime list up to 200 (enough for r ≤ 142) */
    static bool initialized = false;
    static int primes[200];
    static size_t prime_cnt = 0;
    if (!initialized) {
        const size_t LIMIT = 200;
        bool is_prime[LIMIT + 1];
        for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (size_t i = 2; i * i <= LIMIT; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_prime[j] = false;
            }
        }
        for (size_t i = 2; i <= LIMIT; ++i)
            if (is_prime[i]) primes[prime_cnt++] = (int)i;
        initialized = true;
    }

    /* Find row r (1‑based): smallest r with r(r+1)/2 ≥ n */
    idx_t lo = 1, hi = 1;
    while (( (__int128)hi * (hi + 1) ) / 2 < n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val >= n) hi = mid;
        else lo = mid + 1;
    }
    idx_t r = lo;

    /* Column k (1‑based) within the row */
    __int128 prev_sum = (__int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_sum;

    /* Compute parameters for T(r,k) */
    idx_t A = r % k;
    idx_t B = k - A;
    idx_t ceil_v = (r + k - 1) / k;   /* ceil(r/k) */
    idx_t floor_v = r / k;           /* floor(r/k) */

    /* Compute result by prime factorisation */
    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        if (p > r) break;               /* primes larger than r not needed */
        unsigned long long e = 0;
        e += exp_factorial(r, p);
        e -= exp_factorial(A, p);
        e -= exp_factorial(B, p);
        e -= A * exp_factorial(ceil_v, p);
        e -= B * exp_factorial(floor_v, p);

        for (unsigned long long j = 0; j < e; ++j)
            result = mul_small(result, (unsigned)p);
    }
    return result;
}
