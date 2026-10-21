/*
 * A248783  Number of integers k^6 that divide n!.
 * https://oeis.org/A248783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{p≤n} (⌊v_p(n!)/6⌋ + 1) where v_p(n!) is the exponent
 *            of prime p in n!
 * Range:     u8 n <= 53, u16 n <= 120, u32 n <= 288, u64 n <= 707,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Build all primes ≤ limit and store them in a dynamically allocated array.
   The array is freed by the caller. The count of primes is written to *cnt_out. */
static void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    size_t sz = (size_t)(limit + 1);
    char *is_comp = (char*)calloc(sz, 1);
    if (!is_comp) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    uint64_t *tmp = (uint64_t*)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (!is_comp[i]) {
            tmp[cnt++] = (uint64_t)i;
            if (i * i <= (size_t)limit) {
                for (size_t j = i * i; j <= (size_t)limit; j += i) {
                    is_comp[j] = 1;
                }
            }
        }
    }
    free(is_comp);
    *primes_out = tmp;
    *cnt_out = cnt;
}

/* Return the number of integers k^6 dividing n!. */
seq_t a(idx_t n)
{
    /* Offset: a(1) = 1 (handled automatically below). */
    if (n < 2)
        return 1;

    /* Cache primes up to the largest n seen so far. */
    static idx_t primes_upto = 1;
    static uint64_t *primes = NULL;
    static size_t primes_count = 0;

    if (n > primes_upto) {
        free(primes);
        build_primes_upto(n, &primes, &primes_count);
        primes_upto = n;
    }

    seq_t result = 1;
    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        idx_t tmp_n = n;
        idx_t e = 0;
        while (tmp_n) {
            tmp_n /= p;
            e += tmp_n;
        }
        seq_t factor = e / 6 + 1;   /* floor(e/6) + 1 */
        result *= factor;
    }
    return result;
}
