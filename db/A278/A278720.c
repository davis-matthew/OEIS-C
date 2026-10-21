/*
 * A278720  The p-defect p - N(p) of the congruence y^2 == x^3 + 4*x (mod p)
 *          for primes p, where N(p) is the number of solutions given by
 *          A276730.
 * https://oeis.org/A278720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   A278720 – a(n) = 0 if the n‑th prime p ≡ 2 or 3 (mod 4);
 *            otherwise let p = a^2 + 4·b^2 with a odd. Then a(n) =
 *            (-1)^{(a−1)/2 + b} · 2·a
 * Range:     i8 n <= 20, i16 n <= 4599, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   build_primes, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/floor_sqrt.h"

/* cached list of primes */
static idx_t *prime_list = NULL;
static size_t prime_cnt  = 0;
static idx_t   prime_lim  = 0;

/* ensure that the first `need` primes are available */
static void ensure_primes(idx_t need)
{
    if (prime_cnt >= need) return;

    /* start with a reasonable upper bound for the nth prime */
    idx_t lim = (need * 20u) + 200u;
    if (lim < 200u) lim = 200u;

    /* rebuild the list until we have enough primes */
    while (prime_cnt < need) {
        if (prime_list) {
            free(prime_list);
            prime_list = NULL;
        }
        build_primes(lim, &prime_list, &prime_cnt);
        prime_lim = lim;
        if (prime_cnt >= need) break;
        /* increase the bound and retry */
        lim *= 2u;
    }
}

seq_t a(idx_t n)
{
    /* we need at least n primes (1‑based) */
    ensure_primes(n);

    /* retrieve the n‑th prime; prime_list is 0‑based */
    seq_t p = (seq_t)prime_list[n - 1];

    /* zero for primes ≡ 2 or 3 (mod 4) */
    if ((p % 4ULL) == 2 || (p % 4ULL) == 3) {
        return 0;
    }

    /* find representation p = a^2 + 4·b^2 with a odd */
    seq_t a_val = 0;
    seq_t b_val = 0;
    seq_t limit  = floor_sqrt(p);

    for (seq_t a_candidate = 1; a_candidate <= limit; a_candidate += 2) {
        seq_t rem = p - a_candidate * a_candidate;
        if (rem % 4ULL != 0) continue;
        seq_t b2 = rem / 4ULL;
        seq_t b_try = floor_sqrt(b2);
        if (b_try * b_try == b2) {
            a_val = a_candidate;
            b_val = b_try;
            break;
        }
    }

    /* if not found (should not happen for primes ≡ 1 mod 4) */
    if (a_val == 0) return 0;

    /* compute the sign (-1)^((a-1)/2 + b) */
    seq_t parity = ((a_val - 1) / 2 + b_val) & 1;
    seq_t sign   = (parity == 0) ? 1 : -1;

    return sign * (seq_t)2 * a_val;
}
