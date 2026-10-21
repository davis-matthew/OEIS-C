/*
 * A288616  Primes of the form k!6+27, where k!6 is the sextuple factorial
 *          number (A085158).
 * https://oeis.org/A288616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 7, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A288616: primes of the form k!6 + 27, where k!6 is the sextuple
 * factorial (product of all integers ≤k that are congruent to k mod 6).  */
/* This implementation incrementally constructs the sextuple factorial for each
 * k and tests candidate = k!6 + 27 for primality using is_prime_big.
 * Results are memoised across calls so that any n ≥ 1 may be requested in
 * any order. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Memoised array of found primes */
    static seq_t *primes = NULL;
    static size_t prime_count = 0;
    static size_t prime_cap   = 0;

    /* Current sextuple factorial products for residues 0‑5, initialised to 1 */
    static seq_t prod[6] = {1,1,1,1,1,1};

    /* Current k value */
    static idx_t k = 1;

    /* Generate primes until we have at least n of them */
    while (prime_count < n) {
        int r = (int)(k % 6);
        /* Update the product for this residue with the current k */
        prod[r] = prod[r] * (seq_t)k;

        /* Candidate prime */
        seq_t cand = prod[r] + (seq_t)27;

        if (is_prime_big(cand)) {
            /* Resize array if necessary */
            if (prime_count == prime_cap) {
                size_t newcap = prime_cap ? prime_cap * 2 : 1;
                grow_seq(&primes, &prime_cap, newcap);
            }
            primes[prime_count++] = cand;
        }
        ++k;
    }

    return primes[n - 1];
}
