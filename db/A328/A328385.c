/*
 * A328385  If n is of the form p^p, a(n) = n, otherwise a(n) is the first
 *          number found by iterating the map x -> A003415(x) that is
 *          different from n and either a prime, or whose degree (A051903)
 *          differs from the degree of n.
 * https://oeis.org/A328385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 728, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 109 terms through n = 9955 (2026-11-03)
 * Helpers:   factor_u64, is_prime_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328385: Arithmetic derivative iteration
   For n ≥ 1:
     * If n = 1 return 0
     * If n is prime return 1
     * If n = p^p return n
     * Otherwise iterate x → A003415(x) until either
         – the value is prime – return that prime
         – the degree (maximum prime exponent) differs from that of the
           initial n – return that value
     * If the iteration reaches a fixed point, return that value.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_seq.h"
#include "../utils/is_prime_u64.h"

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

/* compute maximum prime exponent (degree) of n using a pre‑factored array */
static inline uint64_t degree_from_factors(const uint64_t *ex, size_t cnt)
{
    uint64_t m = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] > m) m = ex[i];
    }
    return m;
}

/* compute the arithmetic derivative of a uint64_t number */
static inline uint64_t derivative_uint64(uint64_t n,
                                         const uint64_t *pr,
                                         const uint64_t *ex,
                                         size_t cnt)
{
    uint64_t d = 0;
    for (size_t i = 0; i < cnt; ++i) {
        d += (n / pr[i]) * ex[i];
    }
    return d;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    uint64_t cur = (uint64_t)n;

    /* prime test */
    if (is_prime_seq(cur) || is_prime_big((seq_t)cur))
        return (seq_t)1;

    /* factor current number */
    uint64_t pr[20], ex[20];
    size_t cnt;
    factor_u64(cur, pr, ex, &cnt);

    /* p^p test */
    if (cnt == 1 && pr[0] == ex[0])
        return (seq_t)cur;

    /* initial degree */
    uint64_t orig_deg = degree_from_factors(ex, cnt);

    while (1) {
        /* arithmetic derivative */
        uint64_t der = derivative_uint64(cur, pr, ex, cnt);

        /* fixed point check */
        if (der == cur)
            return (seq_t)cur;

        /* prime check */
        if (is_prime_seq(der) || is_prime_big((seq_t)der))
            return (seq_t)der;

        /* factor derivative for next check */
        factor_u64(der, pr, ex, &cnt);
        uint64_t der_deg = degree_from_factors(ex, cnt);

        /* degree difference check */
        if (der_deg != orig_deg)
            return (seq_t)der;

        cur = der; /* continue with new current value */
    }
}
