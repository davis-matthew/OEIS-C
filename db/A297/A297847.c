/*
 * A297847  Sexiness of p = prime(n): number of iterations of the function
 *          f(x) = x + 6 that leave p prime.
 * https://oeis.org/A297847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sexiness sequence:
   a(n) = number of successive applications of f(x) = x + 6 that
   keep the successive values prime, starting from the n‑th prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/is_prime_u64.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Cached list of primes up to the current limit. */
static idx_t *primes = NULL;
static size_t primes_cnt = 0;

/* Ensure that the cached array contains at least n primes. */
static void
ensure_nth_prime(idx_t n)
{
    if (primes_cnt >= n)
        return;                 /* already enough primes */

    free(primes);                /* drop old list */
    primes_cnt = 0;
    primes = NULL;

    /* _prime_limit_for_idx guarantees that the n-th prime is ≤ limit. */
    idx_t limit = _prime_limit_for_idx(n);
    build_primes(limit, &primes, &primes_cnt);
    /* primes_cnt will be ≥ n by the definition of _prime_limit_for_idx. */
}

/* Return the sexiness of the n-th prime. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1; handle out‑of‑range safely */

    /* Obtain prime(n). */
    ensure_nth_prime(n);
    seq_t p = (seq_t)primes[n - 1];

    /* Count how many successive +6 steps remain prime. */
    seq_t iter = 0;
    seq_t curr = p;
    while (true) {
        seq_t next = curr + (seq_t)6;
        if (is_prime_big(next)) {
            iter++;
            curr = next;
        } else {
            break;
        }
    }
    return iter;
}
