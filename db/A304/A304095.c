/*
 * A304095  a(n) is the number of the proper divisors of n that are Lucas
 *          numbers larger than 3 (4, 7, 11, 18, ...).
 * https://oeis.org/A304095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of proper divisors d of n, with d>3 and d a Lucas
 *            number
 * Range:     u8 n <= 9145, u16 n <= 9145, u32 n <= 9145, u64 n <= 9145,
 *            u128 n <= 9145, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This solution computes the result by prime factorisation of n and
   recursion over the divisors.  Lucas numbers >3 are pre‑computed once
   up to UINT64_MAX using unsigned __int128 for the addition.  Only
   standard headers are used; no external helper functions are required. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* --------------------------------------------------------------------
 * Pre‑compute Lucas numbers greater than 3.  The sequence is
 *     L0 = 2,  L1 = 1,  L(n) = L(n-1)+L(n-2)
 * All values that fit in uint64_t are stored.  The count is tiny
 * (only 93 numbers).  The array is filled on the first call of a().
 * -------------------------------------------------------------------- */
static uint64_t lucas_vals[128];
static size_t lucas_cnt = 0;
static bool lucas_ready = false;

static void init_lucas(void)
{
    uint64_t a = 2;          /* L0 */
    uint64_t b = 1;          /* L1 */
    unsigned __int128 sum;

    /* store L2, L3, ... while they fit in uint64_t */
    while (1) {
        sum = (unsigned __int128)a + b;   /* next = L(n) = L(n-1)+L(n-2) */
        if (sum > UINT64_MAX)
            break;
        uint64_t next = (uint64_t)sum;
        if (next > 3) {
            lucas_vals[lucas_cnt++] = next;
        }
        a = b;
        b = next;
    }
}

static inline bool is_lucas(uint64_t x)
{
    if (x <= 3)
        return false;
    /* linear search is fine – list size < 100 */
    for (size_t i = 0; i < lucas_cnt; ++i)
        if (lucas_vals[i] == x)
            return true;
    return false;
}

/* --------------------------------------------------------------------
 * Factorisation of n by simple trial division.
 * The function fills primes[] and exps[] and returns the count of prime
 * factors.  For n <= 10^12 the basic loop is fast; for the intended
 * use (n <= 10 000) it is trivial.
 * -------------------------------------------------------------------- */
static size_t factor_simple(uint64_t n, uint64_t primes[], int exps[])
{
    size_t cnt = 0;
    uint64_t p = 2;
    while (p * p <= n) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
        ++p;
    }
    if (n > 1) {           /* remaining prime factor */
        primes[cnt] = n;
        exps[cnt]   = 1;
        ++cnt;
    }
    return cnt;
}

/* --------------------------------------------------------------------
 * Recursive enumeration over all divisors generated from the
 * prime factorization.
 * -------------------------------------------------------------------- */
static void enumerate_divisors(size_t idx,
                               uint64_t current,
                               const uint64_t *primes,
                               const int *exps,
                               size_t cnt,
                               uint64_t n,
                               seq_t *res)
{
    if (idx == cnt) {
        if (current < n && is_lucas(current))
            ++(*res);
        return;
    }

    uint64_t p = primes[idx];
    uint64_t val = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        enumerate_divisors(idx + 1, current * val, primes, exps, cnt, n, res);
        val *= p;
    }
}

/* --------------------------------------------------------------------
 * Main sequence function.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!lucas_ready) {
        init_lucas();
        lucas_ready = true;
    }

    if (n <= 1)
        return 0;

    /* factorise n */
    uint64_t prime_factors[64];
    int exp_factors[64];
    size_t factor_cnt = factor_simple((uint64_t)n, prime_factors, exp_factors);

    seq_t count = 0;
    enumerate_divisors(0, 1, prime_factors, exp_factors,
                       factor_cnt, (uint64_t)n, &count);
    return count;
}
