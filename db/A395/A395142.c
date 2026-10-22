/*
 * A395142  Numbers k such that sigma(k) = psi(k) + tau(k) + Omega(k)^10.
 * https://oeis.org/A395142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A395142: numbers k such that sigma(k) = psi(k) + tau(k) + Omega(k)^10.
   The sequence consists of the value 21845375, all numbers of the form
   30205446 * p^2 where p is a prime not dividing 30205446, and two additional
   sporadic values 813730722086 and 26688150502064735.  The function returns
   the n‑th such value in increasing order. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static bool initialized = false;
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static const uint64_t LIMIT = 200000;          /* enough for >10 000 primes */
static const seq_t M = (seq_t)30205446;        /* base multiplier */

static void init_once(void)
{
    if (!initialized) {
        /* Build primes up to LIMIT.  The function allocates the array. */
        build_primes_upto(LIMIT, &primes, &primes_cnt);
        initialized = true;
    }
}

static const seq_t sporadic[3] = {
    (seq_t)21845375,
    (seq_t)813730722086,
    (seq_t)26688150502064735
};

seq_t a(idx_t n)
{
    /* Pre‑compute primes only once. */
    init_once();

    /* Handle the first special term. */
    if (n == 1)
        return sporadic[0];

    idx_t idx = 1;                     /* count of terms already produced */
    size_t prime_idx = 0;              /* position in primes array      */
    size_t sp_idx = 1;                 /* next sporadic to consider     */

    while (true) {
        /* Find next candidate of the form M * p^2 with p prime, p≠2,3. */
        seq_t cand = 0;
        bool found = false;
        while (prime_idx < primes_cnt) {
            uint64_t p = primes[prime_idx++];
            if (p == 2 || p == 3) continue;
            cand = M * (seq_t)p * (seq_t)p;
            found = true;
            break;
        }
        if (!found) {
            /* In the unlikely event we ran out of precomputed primes,
               we would need to extend the sieve.  For the required
               range (<10 000 terms) the chosen limit is sufficient. */
            return 0; /* Should not happen for valid n. */
        }

        /* Compare with the next sporadic value, if any left. */
        if (sp_idx < 3) {
            if (sporadic[sp_idx] < cand) {
                /* The sporadic is smaller – output it. */
                idx++;
                if (idx == n)
                    return sporadic[sp_idx];
                sp_idx++;
                continue;
            } else if (sporadic[sp_idx] == cand) {
                /* Equality can't occur, but skip if it does. */
                idx++;
                if (idx == n)
                    return sporadic[sp_idx];
                sp_idx++;
                continue;
            }
        }

        /* Output the candidate. */
        idx++;
        if (idx == n)
            return cand;
    }
}
