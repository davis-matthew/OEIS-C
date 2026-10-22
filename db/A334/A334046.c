/*
 * A334046  a(n) is the number of points of the set { (k, prime(k)), k =
 *          1..n-1 } that are not visible from the point (n, prime(n)) (where
 *          prime(n) denotes the n-th prime number).
 * https://oeis.org/A334046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-1 - A375421(n) (number of invisible points)
 * Range:     u8 n <= 54, u16 n <= 7269, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/isPrimeTrial.h"

/* Forward declaration of helper to generate primes on demand */
static void ensure_primes(idx_t n);

/* Global arrays for primes and cache */
static seq_t *prime_seq = NULL;          /* primes[1..] , 1‑based index */
static idx_t prime_seq_len = 0;          /* number of primes stored */
static idx_t prime_last_checked = 2;     /* last integer checked for primality */

/*
 * Ensure that we have the first `n` primes in `prime_seq`.
 * Uses trial division via isPrimeTrial on small integers.
 */
static void ensure_primes(idx_t n)
{
    if (prime_seq_len >= n) return;

    /* Allocate or grow the array */
    if (!prime_seq) {
        prime_seq = (seq_t*)malloc(sizeof(seq_t) * (n + 4));
        if (!prime_seq) return;          /* malloc failure, should not happen */
        prime_seq[1] = (seq_t)2;
        prime_seq_len = 1;
        prime_last_checked = 2;
    } else {
        /* enlarge to accommodate at least n elements */
        size_t old_cap = prime_seq_len + 4;
        if (old_cap < n + 4) old_cap = n + 4;
        prime_seq = (seq_t*)realloc(prime_seq, sizeof(seq_t) * old_cap);
    }

    for (uint64_t cand = prime_last_checked + 1;
         prime_seq_len < n;
         cand++) {
        if (isPrimeTrial((seq_t)cand)) {
            prime_seq_len++;
            prime_seq[prime_seq_len] = (seq_t)cand;
        }
    }
    prime_last_checked = prime_seq_len ? (uint64_t)prime_seq[prime_seq_len] : 2;
}

/*
 * Main function computing a(n).
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for out of range */
    if (n == 1) return 0;  /* a(1) = 0 by definition */

    /* make sure primes up to n are available */
    ensure_primes(n);

    seq_t visible = 0;

    for (idx_t i = 1; i < n; ++i) {
        seq_t pi = prime_seq[i];
        seq_t pn = prime_seq[n];
        seq_t deltaP = pn - pi;
        seq_t deltaK = (seq_t)(n - i);

        seq_t g = gcd(deltaP, deltaK);
        seq_t num = deltaP / g;
        seq_t den = deltaK / g;          /* den > 0 */

        bool visible_i = true;

        /* If den == deltaK (g==1), the only possible j is j=i+den=n,
           which is not < n, so no inner loop needed. */
        if (den != deltaK) {
            idx_t step = (idx_t)den;     /* den fits into idx_t */
            if (step == 0) continue;    /* safety */

            seq_t expected_p = pi + num; /* first j = i+step corresponds to m=1 */
            for (idx_t j = i + step; j < n; j += step) {
                if (prime_seq[j] == expected_p) {
                    visible_i = false;
                    break;
                }
                expected_p += num;
            }
        }
        if (visible_i) visible++;
    }

    seq_t total = (seq_t)(n - 1);
    return total - visible;
}
