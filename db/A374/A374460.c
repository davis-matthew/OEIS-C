/*
 * A374460  Indices of the nonsquarefree terms in the sequence of
 *          exponentially odd numbers (A268335).
 * https://oeis.org/A374460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 8986, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   OEIS A374460: Indices of the nonsquarefree terms in
   the sequence of exponentially odd numbers (A268335).
   The function returns the nth such index (n >= 1).
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* offset is 1; this case is unused */

    /* Estimate an upper bound for the search: enough for n up to 10000 */
    idx_t limit = n * 20 + 1000;          /* generous cushion */

    /* Compute primes up to sqrt(limit) (needed for factorisation) */
    idx_t prime_limit = 0;
    while ((prime_limit + 1) * (prime_limit + 1) <= limit) ++prime_limit;

    /* Sieve of Eratosthenes for primes up to prime_limit */
    bool *is_comp = (bool *)calloc((size_t)(prime_limit + 1), sizeof(bool));
    if (is_comp == NULL) return (seq_t)0;   /* allocation failure (unlikely) */

    /* Store primes in a small dynamic array */
    idx_t primes[2000];
    size_t prime_cnt = 0;
    for (idx_t p = 2; p <= prime_limit; ++p) {
        if (!is_comp[p]) {
            primes[prime_cnt++] = (idx_t)p;
            if (prime_cnt == 2000) break;    /* safety, though not needed for our limits */
            for (idx_t q = p * p; q <= prime_limit; q += p) {
                is_comp[q] = true;
            }
        }
    }
    free(is_comp);

    /* Scan integer candidates */
    idx_t pos = 0;   /* current index in A268335 */
    idx_t count = 0; /* number of nonsquarefree terms found */
    for (idx_t cand = 1; ; ++cand) {
        idx_t temp = cand;
        bool all_odd = true;
        bool has_ge3 = false;

        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = primes[i];
            if (p * p > temp) break;
            if (temp % p == 0) {
                idx_t exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                if ((exp & 1) == 0) {        /* even exponent -> not exponentially odd */
                    all_odd = false;
                    break;
                }
                if (exp >= 3) has_ge3 = true;
            }
        }
        if (!all_odd) continue;   /* not in sequence A268335 */

        /* Remaining temp is 1 or a prime with exponent 1 (odd) */
        /* If this candidate is nonsquarefree, record its index */
        ++pos;
        if (has_ge3) {
            ++count;
            if (count == n) return (seq_t)pos;
        }
    }
}
