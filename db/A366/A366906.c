/*
 * A366906  The largest exponentially evil divisor of n.
 * https://oeis.org/A366906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 342, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Π over prime powers p^e dividing n of
 *                p^max{k ≤ e | popcount(k) is even}  (evil exponent)
 *          (a(1) = 1)
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

enum { MAX_EXP = 64 };        /* largest exponent to consider for 64‑bit n */
static int ev_exps[MAX_EXP + 1];
static bool ev_initialized = false;

static void init_ev_exps(void)
{
    for (int e = 0; e <= MAX_EXP; ++e) {
        int best = 0;
        for (int k = e; k >= 1; --k) {
            if (__builtin_popcount((unsigned)k) % 2 == 0) {
                best = k;
                break;
            }
        }
        ev_exps[e] = best;   /* 0 if no evil exponent ≤ e */
    }
    ev_initialized = true;
}

/* Return the largest exponentially evil divisor of n */
seq_t a(idx_t n)
{
    if (!ev_initialized) init_ev_exps();

    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    uint64_t t = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Factor out 2 separately, then test odd primes */
    int exp = 0;
    while ((t & 1) == 0) {                /* exponent of 2 */
        t >>= 1;
        ++exp;
    }
    if (exp > 0) {
        int best = ev_exps[exp];
        if (best > 0) {
            seq_t powp = (seq_t)1;
            for (int i = 0; i < best; ++i) powp *= (seq_t)2;
            result *= powp;
        }
    }

    /* Odd prime factors */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            exp = 0;
            while (t % p == 0) {
                t /= p;
                ++exp;
            }
            int best = ev_exps[exp];
            if (best > 0) {
                seq_t powp = (seq_t)1;
                for (int i = 0; i < best; ++i) powp *= (seq_t)p;
                result *= powp;
            }
        }
    }

    /* Remaining prime > sqrt(original n), exponent = 1 */
    if (t > 1) {
        /* exponent 1 is never evil (popcount(1)=1 odd), so skip */
    }

    return result;
}
