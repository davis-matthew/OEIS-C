/*
 * A267134  a(n) = n minus the number of primes of form 6m + 1 that are less
 *          than n-th prime of form 6m - 1.
 * https://oeis.org/A267134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n – #{p < qₙ : p ≡ 1 (mod 6)} where qₙ is the nth prime ≡
 *            5 (mod 6). The function sieves primes up to a limit that
 *            guarantees at least n such primes, then counts the needed
 *            quantities
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------
   Computes the n-th prime that is congruent to 5 (mod 6).
   The sieve size is increased until enough such primes are found.
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;            /* defined only for n≥1, but safe */

    /* Initial upper bound – generous enough for n ≤ 10 000. */
    idx_t limit = n * 30 + 100;      /* +100 is a safety margin */
    while (1) {
        /* Allocate a small array to mark composites. */
        bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
        if (!is_comp)
            return (seq_t)0;        /* allocation failure – shouldn't happen */

        /* Sieve primes up to 'limit'. */
        for (idx_t p = 2; p * p <= limit; ++p) {
            if (is_comp[p])
                continue;
            for (idx_t multiple = p * p; multiple <= limit; multiple += p)
                is_comp[multiple] = true;
        }

        /* Find the n-th prime ≡ 5 (mod 6). */
        idx_t count5 = 0;
        uint64_t nth_prime5 = 0;     /* will hold qₙ */
        for (idx_t p = 2; p <= limit; ++p) {
            if (is_comp[p])
                continue;
            if ((p % 6) == 5) {
                ++count5;
                if (count5 == n) {
                    nth_prime5 = p;
                }
            }
        }

        /* If we did not find enough 5‑mod‑6 primes, enlarge the sieve. */
        if (nth_prime5 == 0) {
            free(is_comp);
            limit *= 2;              /* double the range and retry */
            continue;
        }

        /* Count primes ≡ 1 (mod 6) that are strictly less than nth_prime5. */
        idx_t count1 = 0;
        for (idx_t p = 2; p < nth_prime5; ++p) {
            if (is_comp[p])
                continue;
            if ((p % 6) == 1)
                ++count1;
        }

        free(is_comp);
        return (seq_t)n - (seq_t)count1;
    }
}
